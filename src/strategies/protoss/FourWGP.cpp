// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

#include "FourWGP.h"
#include "Historican.h"

namespace {
Historican gHistory("strategy.four_wgp");
}  // namespace

FourWGP::FourWGP(): Strategy(8.0f) {
}

void FourWGP::OnGameStart(Builder* builder_) {
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_PYLON);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_PYLON);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE);
    builder_->ScheduleObligatoryOrder(sc2::UPGRADE_ID::WARPGATERESEARCH);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
}

void FourWGP::OnUnitIdle(const sc2::Unit* unit_, Builder* builder_) {
    if (unit_->unit_type.ToType() != sc2::UNIT_TYPEID::PROTOSS_GATEWAY &&
            unit_->unit_type.ToType() != sc2::UNIT_TYPEID::PROTOSS_WARPGATE)
        return;

    builder_->ScheduleOptionalOrder(sc2::UNIT_TYPEID::PROTOSS_ZEALOT, unit_);
    gHistory.info() << "Schedule Zealot training" << std::endl;
}
