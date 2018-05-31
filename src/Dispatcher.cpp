// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "Dispatcher.h"
#include "Helpers.h"
#include "Historican.h"
#include "Timer.h"
#include "World.h"
#include "plugins/ChatterBox.h"
#include "plugins/Diagnosis.h"
#include "plugins/ForceCommander.h"
#include "plugins/Governor.h"
#include "plugins/Miner.h"

#include <sc2api/sc2_common.h>
#include <sc2api/sc2_unit.h>

Dispatcher::Dispatcher(): m_builder(new Builder()) {
    gAPI.reset(new API::Interface(Actions(), Control(), Debug(), Observation(), Query()));
    gWorld.reset(new World());

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
        ": construction complete" << std::endl;
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
    // Could be just a worker exiting a refinery.
    if (IsGasWorker()(*unit_))
        return;

    gHistory << sc2::UnitTypeToName(unit_->unit_type) <<
        " was created" << std::endl;

    gWorld->OnUnitCreated(*unit_);

    for (const auto i : m_plugins)
        i->OnUnitCreated(unit_);
}

void Dispatcher::OnUnitIdle(const sc2::Unit* unit_) {
    for (const auto i : m_plugins)
        i->OnUnitIdle(unit_);
}

void Dispatcher::OnUnitDestroyed(const sc2::Unit* unit_) {
    gHistory << sc2::UnitTypeToName(unit_->unit_type) <<
        " was destroyed" << std::endl;
}

void Dispatcher::OnError(const std::vector<sc2::ClientError>& client_errors,
        const std::vector<std::string>& protocol_errors) {
    for (const auto i : client_errors)
        gHistory << "Encountered client error: " << static_cast<int>(i) << std::endl;

    for (const auto i : protocol_errors)
        gHistory << "Encountered protocol error: " << i << std::endl;
}
