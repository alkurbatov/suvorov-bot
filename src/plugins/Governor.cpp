// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../Historican.h"
#include "../Hub.h"
#include "Governor.h"
#include "core/Helpers.h"

Governor::Governor(const std::shared_ptr<Builder>& builder_):
    Plugin(), m_builder(builder_) {
}

void Governor::OnGameStart() {
    auto builder = m_builder.lock();
    if (!builder)
        return;

    // Initial build order
    switch (gHub->GetCurrentRace()) {
        case sc2::Race::Terran:
            gHistory.info() << "Started game as Terran" << std::endl;
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_REFINERY);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
            return;

        case sc2::Race::Zerg:
            gHistory.info() << "Started game as Zerg" << std::endl;
            // NOTE (alkurbatov): Here we use 'ScheduleConstruction' for creatures
            // in order to support proper build order. The training queue is much faster
            // and always has priority over the construction queue.
            // Zergling flood.
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_OVERLORD);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_HATCHERY);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_SPAWNINGPOOL);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_EXTRACTOR);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_QUEEN);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_OVERLORD);
            builder->ScheduleUpgrade(sc2::UPGRADE_ID::ZERGLINGMOVEMENTSPEED);
            return;

        default:
            gHistory.info() << "Started game as Protoss" << std::endl;
            // 4 wgp push
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_PYLON);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_PYLON);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE);
            builder->ScheduleUpgrade(sc2::UPGRADE_ID::WARPGATERESEARCH);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
            return;
    }
}

void Governor::OnStep() {
}

void Governor::OnUnitIdle(const sc2::Unit* unit_) {
    auto builder = m_builder.lock();
    if (!builder)
        return;

    if (unit_->unit_type.ToType() == sc2::UNIT_TYPEID::TERRAN_BARRACKS) {
        builder->ScheduleTraining(sc2::UNIT_TYPEID::TERRAN_MARINE, unit_);
        gHistory.info() << "Schedule Marine training" << std::endl;
        return;
    }

    if (unit_->unit_type.ToType() == sc2::UNIT_TYPEID::PROTOSS_GATEWAY) {
        builder->ScheduleTraining(sc2::UNIT_TYPEID::PROTOSS_ZEALOT, unit_);
        gHistory.info() << "Schedule Zealot training" << std::endl;
        return;
    }

    if (unit_->unit_type.ToType() == sc2::UNIT_TYPEID::ZERG_LARVA) {
        if (gHub->GetLarvaCount() <= builder->GetTrainingOrders().size())
            return;

        builder->ScheduleTraining(sc2::UNIT_TYPEID::ZERG_ZERGLING);
        gHistory.info() << "Schedule Zerglings training" << std::endl;
        return;
    }
}

void Governor::OnUnitDestroyed(const sc2::Unit* unit_) {
    if (unit_->alliance != sc2::Unit::Alliance::Self)
        return;

    if (IsCombatUnit()(*unit_))
        return;

    auto builder = m_builder.lock();
    if (!builder)
        return;

    switch (unit_->unit_type.ToType()) {
        case sc2::UNIT_TYPEID::PROTOSS_PYLON:
        case sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT:
        case sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOTLOWERED:
            // NOTE (alkurbatov): QuarterMaster is responsible for supplies rebuild.
            return;

        case sc2::UNIT_TYPEID::PROTOSS_PROBE:
        case sc2::UNIT_TYPEID::TERRAN_SCV:
        case sc2::UNIT_TYPEID::ZERG_DRONE:
        case sc2::UNIT_TYPEID::ZERG_DRONEBURROWED:
            // NOTE (alkurbatov): Miner is responsible for workers rebuild.
            return;

        case sc2::UNIT_TYPEID::TERRAN_MULE:
        case sc2::UNIT_TYPEID::TERRAN_POINTDEFENSEDRONE:
        case sc2::UNIT_TYPEID::ZERG_EGG:
        case sc2::UNIT_TYPEID::ZERG_LARVA:
            return;

        default:
            builder->ScheduleConstruction(unit_->unit_type.ToType(), true);
            return;
    }
}
