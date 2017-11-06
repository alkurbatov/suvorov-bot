#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <list>
#include <queue>

#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>

#include <sc2api/sc2_agent.h>

#include "Builder.h"
#include "Order.h"

using namespace boost::log::trivial;

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

    std::queue<Order> m_constructionOrders;
    std::list<Order> m_trainingOrders;

    boost::log::sources::severity_logger<severity_level> m_logger;
};

#endif
