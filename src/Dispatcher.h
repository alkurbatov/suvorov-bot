// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

#pragma once

#include "Builder.h"
#include "plugins/Plugin.h"

#include <sc2api/sc2_agent.h>

#include <memory>

// The main bot class.
struct Dispatcher: sc2::Agent {
    explicit Dispatcher(const std::string& opponent_id_);

 private:
    void OnGameStart() final;

    void OnGameEnd() final;

    void OnStep() final;

    void OnBuildingConstructionComplete(const sc2::Unit* building_) final;

    void OnUnitCreated(const sc2::Unit* unit_) final;

    void OnUnitIdle(const sc2::Unit* unit_) final;

    void OnUnitDestroyed(const sc2::Unit* unit_) final;

    void OnUpgradeCompleted(sc2::UpgradeID id_) final;

    void OnUnitEnterVision(const sc2::Unit* unit_) final;

    void OnError(const std::vector<sc2::ClientError>& client_errors,
        const std::vector<std::string>& protocol_errors = {}) final;

    std::shared_ptr<Builder> m_builder;

    std::vector<std::shared_ptr<Plugin>> m_plugins;
};
