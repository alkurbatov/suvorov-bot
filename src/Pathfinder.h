#ifndef PATHFINDER_H
#define PATHFINDER_H

namespace sc2
{
class Unit;
struct Point2D;
} // namespace sc2

struct Pathfinder
{
    static const sc2::Unit* findNearestMineralPatch(const sc2::Point2D& start_);

    static sc2::Point2D getEnemyBaseLocation();
};

#endif
