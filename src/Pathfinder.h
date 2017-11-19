#pragma once

#include <sc2api/sc2_unit.h>

struct Pathfinder {
    // Returns the mineral patch closest to the specified position
    static const sc2::Unit* FindMineralPatch(const sc2::Point2D& start_);

    static sc2::Point2D GetEnemyBaseLocation();
};
