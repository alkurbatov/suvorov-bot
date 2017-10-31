#ifndef BUILDER_H
#define BUILDER_H

#include <boost/core/noncopyable.hpp>

class Builder: boost::noncopyable
{
public:
    Builder(sc2::ActionInterface* action_, const sc2::ObservationInterface* observation_);

    // Find random SCV and try to build a sctructure.
    void buildStructure(const sc2::Point2D &starting_point, sc2::ABILITY_ID ability_type);

private:
    sc2::ActionInterface* m_action;
    const sc2::ObservationInterface* m_observation;
};

#endif
