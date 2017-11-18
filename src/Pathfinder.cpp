#include "API.h"
#include "Pathfinder.h"

#include <sc2api/sc2_map_info.h>
#include <sc2api/sc2_typeenums.h>

const sc2::Unit* Pathfinder::findNearestMineralPatch(const sc2::Point2D& start_)
{
    float distance = std::numeric_limits<float>::max();
    sc2::Units units = gAPI->observer().getUnits(
        sc2::IsUnit(sc2::UNIT_TYPEID::NEUTRAL_MINERALFIELD), sc2::Unit::Alliance::Neutral);

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

sc2::Point2D Pathfinder::getEnemyBaseLocation()
{
    // FIXME: We must scout first, there could be several possible locations.
    return gAPI->observer().gameInfo().enemy_start_locations.front();
}