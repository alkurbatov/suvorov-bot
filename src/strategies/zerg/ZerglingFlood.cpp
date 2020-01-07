// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

#include "Historican.h"
#include "Hub.h"
#include "ZerglingFlood.h"

namespace {
Historican gHistory("strategy.zergling_flood");
}  // namespace

ZerglingFlood::ZerglingFlood(): Strategy(6.0f) {
}

void ZerglingFlood::OnGameStart(Builder* builder_) {
    // NOTE (alkurbatov): Here we use 'ScheduleConstruction' for creatures
    // in order to support proper build order. The training queue is much faster
    // and always has priority over the construction queue.
    // Zergling flood.
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_OVERLORD);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_HATCHERY);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_SPAWNINGPOOL);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_EXTRACTOR);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_QUEEN);
    builder_->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_OVERLORD);
    builder_->ScheduleUpgrade(sc2::UPGRADE_ID::ZERGLINGMOVEMENTSPEED);
}

void ZerglingFlood::OnUnitIdle(const sc2::Unit* unit_, Builder* builder_) {
    if (unit_->unit_type.ToType() != sc2::UNIT_TYPEID::ZERG_LARVA)
        return;

    if (gHub->GetLarvas().Count() <= builder_->GetTrainingOrders().size())
        return;

    builder_->ScheduleTraining(sc2::UNIT_TYPEID::ZERG_ZERGLING);
    gHistory.info() << "Schedule Zerglings training" << std::endl;
}
