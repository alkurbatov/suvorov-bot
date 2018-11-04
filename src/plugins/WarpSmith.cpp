// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "WarpSmith.h"
#include "core/API.h"
#include "core/Helpers.h"

#include <queue>

namespace {
// FIXME (alkurbatov): Unfortunately the API doesn't contain
// real ID for the chrono abilities.
const sc2::ABILITY_ID chronoboost_id = static_cast<sc2::ABILITY_ID>(3755);
const sc2::BUFF_ID chronobuff_id = static_cast<sc2::BUFF_ID>(281);
const int chronoboost_cost = 50;

typedef std::queue<sc2::Unit> targets_t;

targets_t PickTargets() {
    targets_t targets;
    auto units = gAPI->observer().GetUnits();

    for (const auto& i : units()) {
        if (i->unit_type != sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE &&
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

        if (!i->buffs.empty() && i->buffs.front() == chronobuff_id)
            continue;

        targets.push(*i);
    }

    return targets;
}

}  // namespace

WarpSmith::WarpSmith(): m_warp_gates_researched(false) {
}

void WarpSmith::OnUnitIdle(const sc2::Unit* unit_) {
    if (!m_warp_gates_researched ||
        unit_->unit_type.ToType() != sc2::UNIT_TYPEID::PROTOSS_GATEWAY)
        return;

    gAPI->action().OpenGate(*unit_);
}

void WarpSmith::OnStep() {
    auto nexuses = gAPI->observer().GetUnits(IsUnit(sc2::UNIT_TYPEID::PROTOSS_NEXUS));
    targets_t targets = PickTargets();

    for (const auto& i : nexuses()) {
        if (targets.empty())
            return;

        if (i->energy < chronoboost_cost)
            continue;

        gAPI->action().Cast(*i, chronoboost_id, targets.front());
        targets.pop();
    }
}

void WarpSmith::OnUpgradeCompleted(sc2::UpgradeID id_) {
    if (id_ != sc2::UPGRADE_ID::WARPGATERESEARCH)
        return;

    m_warp_gates_researched = true;
}
