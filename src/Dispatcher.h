#pragma once

#include <list>
#include <queue>

#include <sc2api/sc2_agent.h>

#include "Order.h"
#include "Builder.h"
#include "ForceCommander.h"

// The main bot class.
class Dispatcher: public sc2::Agent
{
public:
    Dispatcher();

private:
    virtual void OnGameStart() final;

    virtual void OnStep() final;

    virtual void OnBuildingConstructionComplete(const sc2::Unit* building_) final;

    virtual void OnUnitCreated(const sc2::Unit* unit_) final;

    virtual void OnUnitIdle(const sc2::Unit* unit_) final;

    Builder m_builder;
    ForceCommander m_forceCommander;

    std::queue<Order> m_constructionOrders;
    std::list<Order> m_trainingOrders;
};
