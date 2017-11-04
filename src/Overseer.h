#ifndef OVERSEER_H
#define OVERSEER_H

#include <boost/core/noncopyable.hpp>

class Overseer: boost::noncopyable
{
public:
    explicit Overseer(const sc2::ObservationInterface* observation_);

    size_t countUnitType(sc2::UNIT_TYPEID type_) const;

    bool techRequirementMet(sc2::UNIT_TYPEID req_) const;

    const sc2::Unit* findNearestMineralPatch(const sc2::Point2D& start_) const;

    sc2::Point2D getEnemyStartingLocation() const;

    bool hasSupply() const;

private:
    const sc2::ObservationInterface* m_observation;
};

#endif
