// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "Dispatcher.h"
#include "Historican.h"
#include "Hub.h"
#include "core/API.h"
#include "core/Helpers.h"
#include "core/Timer.h"
#include "plugins/ChatterBox.h"
#include "plugins/Diagnosis.h"
#include "plugins/ForceCommander.h"
#include "plugins/Governor.h"
#include "plugins/Miner.h"
#include "plugins/QuarterMaster.h"

#include <sc2api/sc2_common.h>
#include <sc2api/sc2_unit.h>

Dispatcher::Dispatcher(): m_builder(new Builder()) {
    gAPI.reset(new API::Interface(Actions(), Control(), Debug(), Observation(), Query()));

    m_plugins.emplace_back(new Governor(m_builder));
    m_plugins.emplace_back(new Miner(m_builder));
    m_plugins.emplace_back(new QuarterMaster(m_builder));
    m_plugins.emplace_back(new ForceCommander());

#ifdef DEBUG
    m_plugins.emplace_back(new ChatterBox());
    m_plugins.emplace_back(new Diagnosis(m_builder));
#endif
}

void Dispatcher::OnGameStart() {
    gHistory << "[INFO] New game started!" << std::endl;

    gHub.reset(new Hub(gAPI->observer().GetCurrentRace()));

    for (const auto& i : m_plugins)
        i->OnGameStart();
}

void Dispatcher::OnGameEnd() {
    gHistory << "[INFO] Game over!" <<std::endl;

    for (const auto& i : m_plugins)
        i->OnGameEnd();
}

void Dispatcher::OnBuildingConstructionComplete(const sc2::Unit* building_) {
    gHistory << "[INFO] " << sc2::UnitTypeToName(building_->unit_type) <<
        ": construction complete" << std::endl;
}

void Dispatcher::OnStep() {
    Timer clock;
    clock.Start();

    gHub->OnStep();

    for (const auto& i : m_plugins)
        i->OnStep();

    m_builder->OnStep();

    clock.Finish();
}

void Dispatcher::OnUnitCreated(const sc2::Unit* unit_) {
    // Could be just a worker exiting a refinery.
    if (IsGasWorker()(*unit_))
        return;

    gHistory << "[INFO] " << sc2::UnitTypeToName(unit_->unit_type) <<
        " was created" << std::endl;

    gHub->OnUnitCreated(*unit_);
    m_builder->OnUnitCreated(*unit_);

    for (const auto& i : m_plugins)
        i->OnUnitCreated(unit_);
}

void Dispatcher::OnUnitIdle(const sc2::Unit* unit_) {
    gHub->OnUnitIdle(*unit_);

    for (const auto& i : m_plugins)
        i->OnUnitIdle(unit_);
}

void Dispatcher::OnUnitDestroyed(const sc2::Unit* unit_) {
    gHistory << "[INFO] " << sc2::UnitTypeToName(unit_->unit_type) <<
        " was destroyed" << std::endl;

    gHub->OnUnitDestroyed(*unit_);
}

void Dispatcher::OnError(const std::vector<sc2::ClientError>& client_errors,
        const std::vector<std::string>& protocol_errors) {
    for (const auto i : client_errors) {
        gHistory << "[ERROR] Encountered client error: " <<
            static_cast<int>(i) << std::endl;
    }

    for (const auto& i : protocol_errors)
        gHistory << "[ERROR] Encountered protocol error: " << i << std::endl;
}
