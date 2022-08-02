// The MIT License (MIT)
//
// Copyright (c) 2017-2022 Alexander Kurbatov

#include "Historican.h"
#include "Hub.h"
#include "ZerglingFlood.h"

namespace {
Historican gHistory("strategy.zergling_flood");
}  // namespace

ZerglingFlood::ZerglingFlood(): Strategy(6.0f) {
}

void ZerglingFlood::OnGameStart(Builder* builder_) {
    // NOTE (alkurbatov): Here we use 'ScheduleObligatoryOrder' for creatures
    // in order to support proper build order. The training queue is much faster
    // and always has priority over the construction queue.
    // Zergling flood.
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::ZERG_OVERLORD);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::ZERG_HATCHERY);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::ZERG_SPAWNINGPOOL);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::ZERG_EXTRACTOR);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::ZERG_QUEEN);
    builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::ZERG_OVERLORD);
    builder_->ScheduleObligatoryOrder(sc2::UPGRADE_ID::ZERGLINGMOVEMENTSPEED);
}

void ZerglingFlood::OnUnitIdle(const sc2::Unit* unit_, Builder* builder_) {
    if (unit_->unit_type.ToType() != sc2::UNIT_TYPEID::ZERG_LARVA)
        return;

    uint64_t ordered_lings = static_cast<uint64_t>(
        builder_->CountScheduledOrders(sc2::UNIT_TYPEID::ZERG_ZERGLING));

    if (gHub->GetLarvas().Count() <= ordered_lings)
        return;

    builder_->ScheduleOptionalOrder(sc2::UNIT_TYPEID::ZERG_ZERGLING);
    gHistory.info() << "Schedule Zerglings training\n";
}
