#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <sc2api/sc2_agent.h>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>

#include "Builder.h"
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

    virtual void OnUnitCreated(const sc2::Unit* unit) final;

    virtual void OnUnitIdle(const sc2::Unit* unit) final;

    void TryBuildSupplyDepot();

    void TryBuildBarracks();

    Builder m_builder;
    Overseer m_overseer;

    sc2::Point3D m_startLocation;

    boost::log::sources::severity_logger<severity_level> m_logger;
};

#endif
