#include <sc2api/sc2_map_info.h>
#include <sc2api/sc2_typeenums.h>
#include <sc2api/sc2_interfaces.h>

#include "Overseer.h"

Overseer::Overseer(const sc2::ObservationInterface* observation_):
    m_observation(observation_)
{
}

size_t Overseer::countUnitType(sc2::UNIT_TYPEID unit_type) const
{
    return m_observation->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(unit_type)).size();
}

const sc2::Unit* Overseer::findNearestMineralPatch(const sc2::Point2D& start) const
{
    const sc2::Unit *target = nullptr;
    float distance = std::numeric_limits<float>::max();
    sc2::Units units = m_observation->GetUnits(sc2::Unit::Alliance::Neutral);

    for (const auto &u : units)
    {
        if (u->unit_type != sc2::UNIT_TYPEID::NEUTRAL_MINERALFIELD)
            continue;

        float d = sc2::DistanceSquared2D(u->pos, start);
        if (d < distance)
        {
            distance = d;
            target = u;
        }
    }

    return target;
}

sc2::Point2D Overseer::getEnemyStartingLocation() const
{
    return m_observation->GetGameInfo().enemy_start_locations.front();
}

bool Overseer::hasSupply() const
{
    size_t prediction = countUnitType(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT) * 2 + 2;

    return m_observation->GetFoodUsed() <= m_observation->GetFoodCap() - static_cast<int32_t>(prediction);
}
