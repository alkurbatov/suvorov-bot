// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#include "ChargelotPush.h"
#include "Historican.h"

namespace {
Historican gHistory("strategy.chargelot_push");
}  // namespace

ChargelotPush::ChargelotPush(): Strategy(8.0f), m_build_zealots(false) {
}

void ChargelotPush::OnGameStart(Builder* builder_) {
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_PYLON);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_NEXUS);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_PYLON);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_ADEPT);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL);
    builder_->ScheduleObligatoryOrder(sc2::UPGRADE_ID::WARPGATERESEARCH);
    // NOTE (alkurbatov): We sneed robotics to build a prism but the bot
    // doesn't understand how to use it.
    // builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY);
    builder_->ScheduleObligatoryOrder(sc2::UPGRADE_ID::CHARGE);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_PYLON);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_PYLON);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_PYLON);
}

void ChargelotPush::OnUpgradeCompleted(sc2::UpgradeID) {
    m_build_zealots = true;
}

void ChargelotPush::OnUnitIdle(const sc2::Unit* unit_, Builder* builder_) {
    if (!m_build_zealots)
        return;

    if (unit_->unit_type.ToType() != sc2::UNIT_TYPEID::PROTOSS_GATEWAY &&
            unit_->unit_type.ToType() != sc2::UNIT_TYPEID::PROTOSS_WARPGATE)
        return;

    if (builder_->CountScheduledOrders(sc2::UNIT_TYPEID::PROTOSS_ZEALOT) >= 8)
        return;

    builder_->ScheduleOptionalOrder(sc2::UNIT_TYPEID::PROTOSS_ZEALOT, unit_);
    gHistory.info() << "Schedule Zealot training" << '\n';
}
