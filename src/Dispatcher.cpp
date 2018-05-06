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
#include "plugins/Governor.h"
#include "plugins/Miner.h"

#include <sc2api/sc2_common.h>
#include <sc2api/sc2_unit.h>

Dispatcher::Dispatcher(): m_builder(new Builder()) {
    gAPI.reset(new API::Interface(Actions(), Control(), Debug(), Observation(), Query()));

    m_plugins.emplace_back(new Governor(m_builder));
    m_plugins.emplace_back(new Miner(m_builder));
    m_plugins.emplace_back(new ForceCommander());

#ifdef DEBUG
    m_plugins.emplace_back(new ChatterBox());
    m_plugins.emplace_back(new Diagnosis(m_builder));
#endif
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
    gHistory << sc2::UnitTypeToName(building_->unit_type) <<
        " was created" << std::endl;
}

void Dispatcher::OnStep() {
    Timer clock;
    clock.Start();

    m_builder->OnStep();

    for (const auto i : m_plugins)
        i->OnStep();

    clock.Finish();
}

void Dispatcher::OnUnitCreated(const sc2::Unit* unit_) {
    gHistory << sc2::UnitTypeToName(unit_->unit_type) <<
        " was created" << std::endl;

    for (const auto i : m_plugins)
        i->OnUnitCreated(unit_);
}

void Dispatcher::OnUnitIdle(const sc2::Unit* unit_) {
    for (const auto i : m_plugins)
        i->OnUnitIdle(unit_);

    if (unit_->unit_type.ToType() == sc2::UNIT_TYPEID::TERRAN_BARRACKS)
        m_builder->ScheduleTraining(sc2::UNIT_TYPEID::TERRAN_MARINE, unit_);
}
