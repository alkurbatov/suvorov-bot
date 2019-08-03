// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#include "RepairMan.h"
#include "core/API.h"
#include "core/Helpers.h"

void RepairMan::OnStep(Builder*) {
    if (gAPI->observer().GetCurrentRace() != sc2::Race::Terran)
        return;

    // FIXME (alkuratov): Put buildings repair code here.
}

void RepairMan::OnUnitDestroyed(const sc2::Unit* unit_, Builder* builder_) {
    if (IsCombatUnit()(*unit_))
        return;

    // NOTE (alkurbatov): If build_progress < 1.0f a unit might be
    // destroyed by the CancelConstruction command.
    if (unit_->build_progress < 1.0f)
        return;

    switch (unit_->unit_type.ToType()) {
        case sc2::UNIT_TYPEID::PROTOSS_PYLON:
        case sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT:
        case sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOTLOWERED:
        case sc2::UNIT_TYPEID::ZERG_OVERLORD:
        case sc2::UNIT_TYPEID::ZERG_OVERLORDCOCOON:
        case sc2::UNIT_TYPEID::ZERG_OVERLORDTRANSPORT:
        case sc2::UNIT_TYPEID::ZERG_OVERSEER:
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
        case sc2::UNIT_TYPEID::TERRAN_KD8CHARGE:
        case sc2::UNIT_TYPEID::ZERG_BROODLING:
        case sc2::UNIT_TYPEID::ZERG_EGG:
        case sc2::UNIT_TYPEID::ZERG_LARVA:
            return;

        default:
            builder_->ScheduleConstruction(unit_->unit_type.ToType(), true);
            return;
    }
}
