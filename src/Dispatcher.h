#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <queue>
#include <sc2api/sc2_agent.h>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>

#include "Builder.h"
#include "Order.h"
#include "Overseer.h"

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
    Overseer m_overseer;

    sc2::Point3D m_startLocation;
    std::queue<Order> m_orders;

    boost::log::sources::severity_logger<severity_level> m_logger;
};

#endif
