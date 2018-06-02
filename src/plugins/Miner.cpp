// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "../Helpers.h"
#include "../Order.h"
#include "../Pathfinder.h"
#include "../World.h"
#include "Miner.h"

#include <sc2api/sc2_typeenums.h>

#include <vector>

namespace {

void GatherVespene() {
    auto refineries = gAPI->observer().GetUnits(IsRefinery());

    for ( const auto& i : refineries ) {
        if (i->assigned_harvesters >= i->ideal_harvesters)
            continue;

        auto worker = gAPI->observer().GetClosestUnit(
            i->pos, IsFreeWorker(), sc2::Unit::Alliance::Self);
        if (!worker)
            return;

        gAPI->action().Cast(*worker, sc2::ABILITY_ID::SMART, *i);
    }
}

void CallDownMULE() {
    auto orbitals = gAPI->observer().GetUnits(
        IsUnit(sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND));

    if (orbitals.empty())
        return;

    const sc2::Unit* mineral_target = Pathfinder::FindMineralPatch(
            gAPI->observer().StartingLocation());
    if (!mineral_target)
        return;

    int energy_cost = gAPI->observer().GetUnitTypeData(
        sc2::UNIT_TYPEID::TERRAN_MULE).mineral_cost;

    for ( const auto& i : orbitals ) {
        if (i->energy < energy_cost)
            continue;

        gAPI->action().Cast(*i, sc2::ABILITY_ID::EFFECT_CALLDOWNMULE, *mineral_target);
    }
};

}  // namespace

Miner::Miner(std::shared_ptr<Builder> builder_): Plugin(), m_builder(builder_) {
}

void Miner::OnStep() {
    GatherVespene();

    if (gWorld->GetCurrentRace() == sc2::Race::Terran)
        CallDownMULE();
}

void Miner::OnUnitIdle(const sc2::Unit* unit_) {
    std::vector<Order> orders;

    switch (unit_->unit_type.ToType()) {
        case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER:
        case sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND:
        case sc2::UNIT_TYPEID::TERRAN_PLANETARYFORTRESS:
            // If we can add more SCVs do it.
            if (unit_->assigned_harvesters < unit_->ideal_harvesters) {
                orders.emplace_back(gAPI->observer().GetUnitTypeData(
                    sc2::UNIT_TYPEID::TERRAN_SCV), unit_);
            }

            break;

        case sc2::UNIT_TYPEID::TERRAN_SCV:
        case sc2::UNIT_TYPEID::ZERG_DRONE: {
            const sc2::Unit* mineral_target = Pathfinder::FindMineralPatch(
                gAPI->observer().StartingLocation());
            if (!mineral_target)
                break;

            gAPI->action().Cast(*unit_, sc2::ABILITY_ID::SMART, *mineral_target);
            break;
        }

        default:
            break;
    }

    auto builder = m_builder.lock();
    if (!builder)
        return;

    builder->ScheduleOrders(orders);
}
