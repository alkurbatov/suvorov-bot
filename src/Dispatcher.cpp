// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "Dispatcher.h"
#include "Historican.h"
#include "Timer.h"
#include "plugins/ChatterBox.h"
#include "plugins/Diagnosis.h"
#include "plugins/ForceCommander.h"
#include "plugins/Miner.h"

#include <sc2api/sc2_common.h>
#include <sc2api/sc2_unit.h>

Dispatcher::Dispatcher(): m_builder(new Builder()) {
    gAPI.reset(new API::Interface(Actions(), Control(), Debug(), Observation(), Query()));

    m_plugins.emplace_back(new Miner(m_builder));
    m_plugins.emplace_back(new ForceCommander());
    m_plugins.emplace_back(new ChatterBox());
    m_plugins.emplace_back(new Diagnosis(m_builder));
}

void Dispatcher::OnGameStart() {
    gHistory << "New game started!" << std::endl;

    // Initial build order
    m_builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    m_builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    m_builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    m_builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    m_builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_REFINERY);
    m_builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND);
    m_builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    m_builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    m_builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    m_builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    m_builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);

    for (const auto i : m_plugins)
        i->OnGameStart();
}

void Dispatcher::OnGameEnd() {
    gHistory << "Game over!" <<std::endl;

    for (const auto i : m_plugins)
        i->OnGameEnd();
}

void Dispatcher::OnBuildingConstructionComplete(const sc2::Unit* building_) {
    gHistory << "Loop Step #" << Observation()->GetGameLoop() <<
        ": Building was created, tag: " << building_->tag << std::endl;
}

void Dispatcher::OnStep() {
    Timer clock;
    clock.Start();

    m_builder->OnStep();

    for (const auto i : m_plugins)
        i->OnStep();

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

    clock.Finish();
}

void Dispatcher::OnUnitCreated(const sc2::Unit* unit_) {
    gHistory << "Loop Step #" << Observation()->GetGameLoop() <<
        ": Unit was created, tag: " << unit_->tag << std::endl;

    for (const auto i : m_plugins)
        i->OnUnitCreated(unit_);
}

void Dispatcher::OnUnitIdle(const sc2::Unit* unit_) {
    for (const auto i : m_plugins)
        i->OnUnitIdle(unit_);

    if (unit_->unit_type.ToType() == sc2::UNIT_TYPEID::TERRAN_BARRACKS)
        m_builder->ScheduleTraining(sc2::UNIT_TYPEID::TERRAN_MARINE, unit_);
}
