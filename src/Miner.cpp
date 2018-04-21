// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "Helpers.h"
#include "Miner.h"
#include "Pathfinder.h"

#include <sc2api/sc2_typeenums.h>

void Miner::OnStep() const {
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

    for ( auto i : orbitals ) {
        if (i->energy < energy_cost)
            continue;

        gAPI->action().Cast(*i, sc2::ABILITY_ID::EFFECT_CALLDOWNMULE, *mineral_target);
    }
}
