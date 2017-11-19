// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Order.h"
#include "Builder.h"
#include "ForceCommander.h"

#include <list>
#include <queue>

#include <sc2api/sc2_agent.h>

// The main bot class.
struct Dispatcher: sc2::Agent {
    Dispatcher();

private:
    void OnGameStart() final;

    void OnStep() final;

    void OnBuildingConstructionComplete(const sc2::Unit* building_) final;

    void OnUnitCreated(const sc2::Unit* unit_) final;

    void OnUnitIdle(const sc2::Unit* unit_) final;

    Builder m_builder;
    ForceCommander m_force_commander;

    std::queue<Order> m_construction_orders;
    std::list<Order> m_training_orders;
};
