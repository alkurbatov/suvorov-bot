// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#include "WarpSmith.h"
#include "core/API.h"
#include "core/Helpers.h"

#include <queue>

namespace {
const int chronoboost_cost = 50;

typedef std::queue<sc2::Unit> targets_t;

targets_t PickTargets() {
    targets_t targets;
    auto units = gAPI->observer().GetUnits();

    for (const auto& i : units()) {
        if (i->unit_type != sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE &&
            i->unit_type != sc2::UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL &&
            i->unit_type != sc2::UNIT_TYPEID::PROTOSS_FORGE &&
            i->unit_type != sc2::UNIT_TYPEID::PROTOSS_NEXUS &&
            i->unit_type != sc2::UNIT_TYPEID::PROTOSS_ROBOTICSBAY &&
            i->unit_type != sc2::UNIT_TYPEID::PROTOSS_FLEETBEACON &&
            i->unit_type != sc2::UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY &&
            i->unit_type != sc2::UNIT_TYPEID::PROTOSS_STARGATE &&
            i->unit_type != sc2::UNIT_TYPEID::PROTOSS_WARPGATE
        )
            continue;

        if (i->orders.empty())
            continue;

        if (!i->buffs.empty() && i->buffs.front() == sc2::BUFF_ID::CHRONOBOOSTENERGYCOST)
            continue;

        targets.push(*i);
    }

    return targets;
}

}  // namespace

WarpSmith::WarpSmith(): m_warp_gates_researched(false) {
}

void WarpSmith::OnUnitIdle(const sc2::Unit* unit_, Builder*) {
    if (!m_warp_gates_researched ||
        unit_->unit_type.ToType() != sc2::UNIT_TYPEID::PROTOSS_GATEWAY)
        return;

    gAPI->action().OpenGate(*unit_);
}

void WarpSmith::OnStep(Builder*) {
    auto nexuses = gAPI->observer().GetUnits(IsUnit(sc2::UNIT_TYPEID::PROTOSS_NEXUS));
    targets_t targets = PickTargets();

    for (const auto& i : nexuses()) {
        if (targets.empty())
            return;

        if (i->energy < chronoboost_cost)
            continue;

        gAPI->action().Cast(*i,
            sc2::ABILITY_ID::EFFECT_CHRONOBOOSTENERGYCOST, targets.front());
        targets.pop();
    }
}

void WarpSmith::OnUpgradeCompleted(sc2::UpgradeID id_) {
    if (id_ != sc2::UPGRADE_ID::WARPGATERESEARCH)
        return;

    m_warp_gates_researched = true;
}
