// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "Dispatcher.h"
#include "Historican.h"
#include "Pathfinder.h"
#include "Timer.h"

#include <sc2api/sc2_common.h>
#include <sc2api/sc2_unit.h>

Dispatcher::Dispatcher() {
    gAPI.reset(new API::Interface(Actions(), Control(), Debug(), Observation(), Query()));
}

void Dispatcher::OnGameStart() {
    gHistory << "New game started!" << std::endl;

    // Initial build order
    m_builder.ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    m_builder.ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    m_builder.ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    m_builder.ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    m_builder.ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_REFINERY);
    m_builder.ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND);
    m_builder.ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    m_builder.ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    m_builder.ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    m_builder.ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    m_builder.ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);

    m_chatterbox.OnGameStart();
}

void Dispatcher::OnGameEnd() {
    gHistory << "Game over!" <<std::endl;

    m_diagnosis.OnGameEnd();
}

void Dispatcher::OnBuildingConstructionComplete(const sc2::Unit* building_) {
    gHistory << "Loop Step #" << Observation()->GetGameLoop() <<
        ": Building was created, tag: " << building_->tag << std::endl;
}

void Dispatcher::OnStep() {
    Timer clock;
    clock.Start();

    m_builder.OnStep();
    m_chatterbox.OnStep();
    m_force_commander.OnStep();
    m_miner.OnStep();

    // FIXME: skip this if we've planned additional supply already.
    // FIXME: skip this if we have 200 cap limit.
    // If we are not supply capped, don't build a supply depot.
    // size_t prediction = gAPI->observer().countUnitType(
    //     sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT) * 2 + 2;
    // if (Observation()->GetFoodUsed() <=
    //     Observation()->GetFoodCap() - static_cast<int32_t>(prediction))
    //     return;

    // m_constructionOrders.emplace(
    //    gAPI->observer().GetUnitTypeData(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT));

    m_diagnosis.ShowBuildOrder(m_builder.GetConstructionOrders());
    m_diagnosis.OnStep();

    clock.Finish();
}

void Dispatcher::OnUnitCreated(const sc2::Unit* unit_) {
    gHistory << "Loop Step #" << Observation()->GetGameLoop() <<
        ": Unit was created, tag: " << unit_->tag << std::endl;

    m_force_commander.OnUnitCreated(unit_);
}

void Dispatcher::OnUnitIdle(const sc2::Unit* unit_) {
    switch (unit_->unit_type.ToType()) {
        case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER:
        case sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND:
        case sc2::UNIT_TYPEID::TERRAN_PLANETARYFORTRESS:
            // If we can add more SCVs do it.
            if (unit_->assigned_harvesters < unit_->ideal_harvesters)
                m_builder.ScheduleTraining(sc2::UNIT_TYPEID::TERRAN_SCV, unit_);

            break;

        case sc2::UNIT_TYPEID::TERRAN_SCV: {
            const sc2::Unit* mineral_target = Pathfinder::FindMineralPatch(
                gAPI->observer().StartingLocation());
            if (!mineral_target)
                break;

            Actions()->UnitCommand(unit_, sc2::ABILITY_ID::SMART, mineral_target);
            break;
        }

        case sc2::UNIT_TYPEID::TERRAN_BARRACKS:
            m_builder.ScheduleTraining(sc2::UNIT_TYPEID::TERRAN_MARINE, unit_);
            break;

        default:
            break;
    }
}
