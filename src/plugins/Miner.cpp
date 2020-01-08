// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

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
        if (i->assigned_harvesters >= i->ideal_harvesters)
            continue;

        if (!i->orders.empty())
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

    auto units = gAPI->observer().GetUnits(IsVisibleMineralPatch(),
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

}  // namespace

void Miner::OnStep(Builder* builder_) {
    SecureMineralsIncome(builder_);
    SecureVespeneIncome();

    if (gHub->GetCurrentRace() == sc2::Race::Terran)
        CallDownMULE();
}

void Miner::OnUnitCreated(const sc2::Unit* unit_, Builder*) {
    if (!sc2::IsTownHall()(*unit_))
        return;

    auto units = gAPI->observer().GetUnits(IsVisibleMineralPatch(),
        sc2::Unit::Alliance::Neutral);

    const sc2::Unit* mineral_target = units.GetClosestUnit(unit_->pos);
    if (!mineral_target)
        return;

    gAPI->action().Cast(*unit_, sc2::ABILITY_ID::RALLY_WORKERS, *mineral_target);
}

void Miner::OnUnitIdle(const sc2::Unit* unit_, Builder*) {
    auto units = gAPI->observer().GetUnits(IsVisibleMineralPatch(),
        sc2::Unit::Alliance::Neutral);

    switch (unit_->unit_type.ToType()) {
        case sc2::UNIT_TYPEID::PROTOSS_PROBE:
        case sc2::UNIT_TYPEID::TERRAN_SCV:
        case sc2::UNIT_TYPEID::ZERG_DRONE: {
            const sc2::Unit* mineral_target = units.GetClosestUnit(
                gAPI->observer().StartingLocation());
            if (!mineral_target)
                return;

            gAPI->action().Cast(*unit_, sc2::ABILITY_ID::SMART, *mineral_target);
            break;
        }

        default:
            break;
    }
}
