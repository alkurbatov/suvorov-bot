// The MIT License (MIT)
//
// Copyright (c) 2017-2022 Alexander Kurbatov

#include "Historican.h"
#include "Hub.h"
#include "QuarterMaster.h"
#include "core/API.h"
#include "core/Helpers.h"

#include <numeric>

namespace {
Historican gHistory("plugin.quarter_master");

struct CalcSupplies {
    float operator()(float sum, const sc2::Unit* unit_) const;
};

float CalcSupplies::operator()(float sum, const sc2::Unit* unit_) const {
    switch (unit_->unit_type.ToType()) {
        case sc2::UNIT_TYPEID::PROTOSS_NEXUS:
        case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER:
        case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTERFLYING:
        case sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND:
        case sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMANDFLYING:
        case sc2::UNIT_TYPEID::TERRAN_PLANETARYFORTRESS:
            if (!unit_->IsBuildFinished())
                return sum;
            return sum + 15.0f;

        case sc2::UNIT_TYPEID::ZERG_HATCHERY:
        case sc2::UNIT_TYPEID::ZERG_HIVE:
        case sc2::UNIT_TYPEID::ZERG_LAIR:
            return sum + 6.0f;

        case sc2::UNIT_TYPEID::ZERG_EGG:
            if (unit_->orders.front().ability_id == sc2::ABILITY_ID::TRAIN_OVERLORD)
                return sum + 8.0f;

            return sum;

        case sc2::UNIT_TYPEID::PROTOSS_PYLON:
        case sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT:
        case sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOTLOWERED:
        case sc2::UNIT_TYPEID::ZERG_OVERLORD:
        case sc2::UNIT_TYPEID::ZERG_OVERLORDCOCOON:
        case sc2::UNIT_TYPEID::ZERG_OVERLORDTRANSPORT:
        case sc2::UNIT_TYPEID::ZERG_OVERSEER:
            return sum + 8.0f;

        default:
            return sum;
    }
}

struct CalcConsumptionRate {
    float operator()(float sum, const sc2::Unit* unit_) const;
};

float CalcConsumptionRate::operator()(float sum, const sc2::Unit* unit_) const {
    if (!unit_->IsBuildFinished())
        return sum;

    switch (unit_->unit_type.ToType()) {
        case sc2::UNIT_TYPEID::PROTOSS_NEXUS:
        case sc2::UNIT_TYPEID::ZERG_HATCHERY:
        case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER:
        case sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND:
        case sc2::UNIT_TYPEID::TERRAN_PLANETARYFORTRESS:
            return sum + 1.75f;
            // SCV = 272 framesToTrain (exactly), TrainRate=1/272
            // SupplyDepot = 480 framesToTrain, TrainRate = 1/480
            // SCV's trained while Depot builds = 480/272 = 1.765

        case sc2::UNIT_TYPEID::TERRAN_BARRACKS:
            return sum + 1.2f;
            // MarineTrainTime = 18*22.4 = 403.2 (roughly), TrainRate = 1/403
            // SupplyDepot = 480 framesToTrain, TrainRate = 1/480
            // Marines trained while Depot builds = 480/403 = 1.191

        case sc2::UNIT_TYPEID::TERRAN_BARRACKSREACTOR:
            return sum + 2.4f;
            // Double the rounded 1.2 rate of single Barracks = 2.4

        case sc2::UNIT_TYPEID::TERRAN_BARRACKSTECHLAB:
            return sum + 2.0f;
            // MarauderTrainTime = 21*22.4 = 470.4 (roughly), TrainRate = 1/470
            // SupplyDepot = 480 framesToTrain, TrainRate = 1/480
            // Marauders trained while Depot builds = 480/470 = 1.02
            // Marauder is 2 supply, so round to 2.0 overall

            case sc2::UNIT_TYPEID::TERRAN_FACTORY:
            case sc2::UNIT_TYPEID::TERRAN_FACTORYTECHLAB:
            case sc2::UNIT_TYPEID::TERRAN_FACTORYREACTOR:
            case sc2::UNIT_TYPEID::TERRAN_STARPORT:
            case sc2::UNIT_TYPEID::TERRAN_STARPORTTECHLAB:
            case sc2::UNIT_TYPEID::TERRAN_STARPORTREACTOR:
            case sc2::UNIT_TYPEID::ZERG_LAIR:
            case sc2::UNIT_TYPEID::ZERG_HIVE:
            case sc2::UNIT_TYPEID::PROTOSS_GATEWAY:
            case sc2::UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY:
            case sc2::UNIT_TYPEID::PROTOSS_STARGATE:
                return sum + 2.0f;  // just add 2.0 for all others for now

        default:
            return sum;
    }
}


}  // namespace

QuarterMaster::QuarterMaster():
    Plugin(), m_skip_until_frame(0) {
}

void QuarterMaster::OnStep(Builder* builder_) {
    if (m_skip_until_frame > gAPI->observer().GetGameLoop())
        return;

    auto units = gAPI->observer().GetUnits();
    std::list<Order> orders = builder_->GetOrders();

    if (!orders.empty()
        && orders.begin()->unit_type_id == gHub->GetCurrentSupplyType()) {
        m_skip_until_frame = gAPI->observer().GetGameLoop() + m_frames_to_skip;
        return;  // wait 10 seconds if already scheduled Supply
    }

    float expected_consumption = gAPI->observer().GetFoodUsed()
        + std::accumulate(units().begin(), units().end(), 0.0f, CalcConsumptionRate());

    float expected_supply =
        std::accumulate(units().begin(), units().end(), 0.0f, CalcSupplies());

    if (expected_supply > expected_consumption || expected_supply >= 200.0f)
        return;

    gHistory.info() << "Request additional supplies: " <<
        expected_consumption << " >= " << expected_supply << '\n';

    m_skip_until_frame = gAPI->observer().GetGameLoop() + m_frames_to_skip;

    builder_->ScheduleObligatoryOrder(gHub->GetCurrentSupplyType(), true);
}

void QuarterMaster::OnUnitCreated(const sc2::Unit* unit_,  Builder*) {
    if (unit_->unit_type == gHub->GetCurrentSupplyType()) {
        m_skip_until_frame = 0;
    }
}
