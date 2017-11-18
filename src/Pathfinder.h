#pragma once

#include <sc2api/sc2_unit.h>

struct Pathfinder
{
    static const sc2::Unit* findNearestMineralPatch(const sc2::Point2D& start_);

    static sc2::Point2D getEnemyBaseLocation();
};
