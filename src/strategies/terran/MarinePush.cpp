// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#include "Historican.h"
#include "MarinePush.h"
#include "Hub.h"

namespace {
Historican gHistory("strategy.marine_push");
}  // namespace

MarinePush::MarinePush(): Strategy(16.0f) {
}

void MarinePush::OnGameStart(Builder* builder_) {
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::TERRAN_REFINERY);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleOptionalOrder(sc2::UNIT_TYPEID::TERRAN_BARRACKSTECHLAB);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleObligatoryOrder(sc2::UPGRADE_ID::SHIELDWALL);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
}

void MarinePush::OnUnitIdle(const sc2::Unit* unit_, Builder* builder_) {
    if (unit_->unit_type.ToType() != sc2::UNIT_TYPEID::TERRAN_BARRACKS)
        return;

    builder_->ScheduleOptionalOrder(sc2::UNIT_TYPEID::TERRAN_MARINE, unit_);
    gHistory.info() << "Schedule Marine training\n";
}
