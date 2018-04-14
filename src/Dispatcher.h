// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Builder.h"
#include "ChatterBox.h"
#include "Diagnosis.h"
#include "ForceCommander.h"

#include <sc2api/sc2_agent.h>

// The main bot class.
struct Dispatcher: sc2::Agent {
    Dispatcher();

 private:
    void OnGameStart() final;

    void OnGameEnd() final;

    void OnStep() final;

    void OnBuildingConstructionComplete(const sc2::Unit* building_) final;

    void OnUnitCreated(const sc2::Unit* unit_) final;

    void OnUnitIdle(const sc2::Unit* unit_) final;

    Builder m_builder;
    ChatterBox m_chatterbox;
    Diagnosis m_diagnosis;
    ForceCommander m_force_commander;
};
