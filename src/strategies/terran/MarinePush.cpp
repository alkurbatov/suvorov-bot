// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

#include "Historican.h"
#include "MarinePush.h"

namespace {
Historican gHistory("strategy.marine_push");
}  // namespace

MarinePush::MarinePush(): Strategy(16.0f) {
}

void MarinePush::OnGameStart(Builder* builder_) {
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_REFINERY);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKSTECHLAB);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    builder_->ScheduleUpgrade(sc2::UPGRADE_ID::SHIELDWALL);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleUpgrade(sc2::UPGRADE_ID::STIMPACK);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKSREACTOR);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKSREACTOR);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKSREACTOR);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKSREACTOR);
}

void MarinePush::OnUnitIdle(const sc2::Unit* unit_, Builder* builder_) {
    if (unit_->unit_type.ToType() != sc2::UNIT_TYPEID::TERRAN_BARRACKS)
        return;

    builder_->ScheduleTraining(sc2::UNIT_TYPEID::TERRAN_MARINE, unit_);
    gHistory.info() << "Schedule Marine training" << std::endl;
}
