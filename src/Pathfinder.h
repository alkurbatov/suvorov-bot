#pragma once

#include <sc2api/sc2_unit.h>

struct Pathfinder {
    // Returns the mineral patch closest to the specified point
    static const sc2::Unit* FindMineralPatch(const sc2::Point2D& point_);

    // Returns the vespene geyser closest to the specified point
    static const sc2::Unit* FindVespeneGeyser(const sc2::Point2D& point_);

    static sc2::Point2D GetEnemyBaseLocation();
};
