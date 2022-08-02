// The MIT License (MIT)
//
// Copyright (c) 2017-2022 Alexander Kurbatov

#include "Hub.h"
#include "Miner.h"
#include "core/API.h"
#include "core/Helpers.h"
#include "core/Order.h"

#include <sc2api/sc2_typeenums.h>
#include <sc2api/sc2_unit_filters.h>

#include <vector>

namespace {
const int mule_energy_cost = 50;

void SecureMineralsIncome(Builder* builder_) {
    auto town_halls = gAPI->observer().GetUnits(sc2::IsTownHall());

    for (const auto& i : town_halls()) {
        if (!i->orders.empty() || !i->IsBuildFinished())
            continue;

        if (i->assigned_harvesters > i->ideal_harvesters)
            continue;

        if (builder_->CountScheduledOrders(gHub->GetCurrentWorkerType()) > 0)
            continue;

        // FIXME (alkurbatov): We should set an assignee for drones
        // and pick a larva closest to the assignee.
        if (gHub->GetCurrentRace() == sc2::Race::Zerg) {
            builder_->ScheduleOptionalOrder(sc2::UNIT_TYPEID::ZERG_DRONE);
            continue;
        }

        builder_->ScheduleOptionalOrder(gHub->GetCurrentWorkerType(), i);
    }
}

void SecureVespeneIncome() {
    auto refineries = gAPI->observer().GetUnits(IsRefinery());

    for (const auto& i : refineries()) {
        if (i->assigned_harvesters >= i->ideal_harvesters)
            continue;

        gHub->AssignVespeneHarvester(*i);
    }
}

void CallDownMULE() {
    auto orbitals = gAPI->observer().GetUnits(
        IsUnit(sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND));

    if (orbitals.Empty())
        return;

    auto units = gAPI->observer().GetUnits(sc2::IsVisibleMineralPatch(),
        sc2::Unit::Alliance::Neutral);

    for (const auto& i : orbitals()) {
        if (i->energy < mule_energy_cost)
            continue;

        const sc2::Unit* mineral_target = units.GetClosestUnit(i->pos);
        if (!mineral_target)
            continue;

        gAPI->action().Cast(*i, sc2::ABILITY_ID::EFFECT_CALLDOWNMULE, *mineral_target);
    }
}

const Expansion* GetBestMiningExpansionNear(const sc2::Unit* unit_) {
    if (!unit_)
        return nullptr;

    sc2::Point2D worker_loc = unit_->pos;
    float unsaturated_dist = std::numeric_limits<float>::max();
    const Expansion* closest_unsaturated = nullptr;
    float saturated_dist = std::numeric_limits<float>::max();
    const Expansion* closest_saturated = nullptr;

    std::vector<const Expansion*> saturated_expansions;

    // Find closest unsaturated expansion
    for (auto& i : gHub->GetExpansions()) {
        if (i.owner != Owner::SELF)
            continue;

        const sc2::Unit* th = gAPI->observer().GetUnit(i.town_hall_tag);
        if (!th->IsBuildFinished())
            continue;

        float dist = sc2::DistanceSquared2D(worker_loc, th->pos);

        if (th->assigned_harvesters >= th->ideal_harvesters) {
            if (dist < saturated_dist) {
                closest_saturated = &i;
                saturated_dist = dist;
            }

            continue;
        }

        if (dist < unsaturated_dist) {
            closest_unsaturated = &i;
            unsaturated_dist = dist;
        }
    }

    if (closest_unsaturated != nullptr)
        return closest_unsaturated;  // Return nearest unsaturated

    return closest_saturated;  // No unsaturated, send to nearest saturated
}

void DistrubuteMineralWorker(const sc2::Unit* unit_) {
    if (!unit_)
        return;

    sc2::Point2D target_loc = gAPI->observer().StartingLocation();
    const Expansion* expansion = GetBestMiningExpansionNear(unit_);
    if (expansion)
        target_loc = expansion->town_hall_location;

    auto patches = gAPI->observer().GetUnits(
        sc2::IsVisibleMineralPatch(), sc2::Unit::Alliance::Neutral);
    const sc2::Unit* mineral_target = patches.GetClosestUnit(target_loc);

    if (!mineral_target)
        return;

    gAPI->action().Cast(*unit_, sc2::ABILITY_ID::SMART, *mineral_target);
}

}  // namespace

void Miner::OnStep(Builder* builder_) {
    SecureMineralsIncome(builder_);
    SecureVespeneIncome();

    if (gHub->GetCurrentRace() == sc2::Race::Terran)
        CallDownMULE();
}

void Miner::OnUnitCreated(const sc2::Unit* unit_, Builder*) {
    if (unit_->unit_type == gHub->GetCurrentWorkerType())
        DistrubuteMineralWorker(unit_);
}

void Miner::OnUnitIdle(const sc2::Unit* unit_, Builder*) {
    if (unit_->unit_type == gHub->GetCurrentWorkerType())
        DistrubuteMineralWorker(unit_);
}
