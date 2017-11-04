#include <sc2api/sc2_map_info.h>
#include <sc2api/sc2_typeenums.h>
#include <sc2api/sc2_interfaces.h>

#include "Overseer.h"
#include "Helpers.h"

Overseer::Overseer(const sc2::ObservationInterface* observation_):
    m_observation(observation_)
{
}

size_t Overseer::countUnitType(sc2::UNIT_TYPEID type_) const
{
    return m_observation->GetUnits(sc2::Unit::Alliance::Self, isUnit(type_)).size();
}

bool Overseer::techRequirementMet(sc2::UNIT_TYPEID req_) const
{
    // Here sc2::UNIT_TYPEID::INVALID means that no tech requirements needed.
    return req_ == sc2::UNIT_TYPEID::INVALID || countUnitType(req_) > 0;
}

const sc2::Unit* Overseer::findNearestMineralPatch(const sc2::Point2D& start_) const
{
    float distance = std::numeric_limits<float>::max();
    sc2::Units units = m_observation->GetUnits(sc2::Unit::Alliance::Neutral,
        sc2::IsUnit(sc2::UNIT_TYPEID::NEUTRAL_MINERALFIELD));

    const sc2::Unit* target = nullptr;
    for (const auto& i : units)
    {
        float d = sc2::DistanceSquared2D(i->pos, start_);
        if (d < distance)
        {
            distance = d;
            target = i;
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
