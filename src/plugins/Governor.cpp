// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../Historican.h"
#include "../Hub.h"
#include "Governor.h"

void Governor::OnGameStart(Builder* builder_) {
    // Initial build order
    switch (gHub->GetCurrentRace()) {
        case sc2::Race::Terran:
            gHistory.info() << "Started game as Terran" << std::endl;
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
            return;

        case sc2::Race::Zerg:
            gHistory.info() << "Started game as Zerg" << std::endl;
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
            return;

        default:
            gHistory.info() << "Started game as Protoss" << std::endl;
            // 4 wgp push
            builder_->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_PYLON);
            builder_->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
            builder_->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR);
            builder_->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_PYLON);
            builder_->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE);
            builder_->ScheduleUpgrade(sc2::UPGRADE_ID::WARPGATERESEARCH);
            builder_->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
            builder_->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
            builder_->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
            return;
    }
}

void Governor::OnStep(Builder*) {
}

void Governor::OnUnitIdle(const sc2::Unit* unit_, Builder* builder_) {
    if (unit_->unit_type.ToType() == sc2::UNIT_TYPEID::TERRAN_BARRACKS) {
        builder_->ScheduleTraining(sc2::UNIT_TYPEID::TERRAN_MARINE, unit_);
        gHistory.info() << "Schedule Marine training" << std::endl;
        return;
    }

    if (unit_->unit_type.ToType() == sc2::UNIT_TYPEID::PROTOSS_GATEWAY ||
            unit_->unit_type.ToType() == sc2::UNIT_TYPEID::PROTOSS_WARPGATE) {
        builder_->ScheduleTraining(sc2::UNIT_TYPEID::PROTOSS_ZEALOT, unit_);
        gHistory.info() << "Schedule Zealot training" << std::endl;
        return;
    }

    if (unit_->unit_type.ToType() == sc2::UNIT_TYPEID::ZERG_LARVA) {
        if (gHub->GetLarvas().Count() <= builder_->GetTrainingOrders().size())
            return;

        builder_->ScheduleTraining(sc2::UNIT_TYPEID::ZERG_ZERGLING);
        gHistory.info() << "Schedule Zerglings training" << std::endl;
        return;
    }
}
