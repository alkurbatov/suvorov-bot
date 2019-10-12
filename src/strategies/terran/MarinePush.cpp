// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

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
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
}

void MarinePush::OnUnitIdle(const sc2::Unit* unit_, Builder* builder_) {
    if (unit_->unit_type.ToType() != sc2::UNIT_TYPEID::TERRAN_BARRACKS)
        return;

    builder_->ScheduleTraining(sc2::UNIT_TYPEID::TERRAN_MARINE, unit_);
    gHistory.info() << "Schedule Marine training" << std::endl;
}
