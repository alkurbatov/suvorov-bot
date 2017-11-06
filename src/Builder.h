#ifndef BUILDER_H
#define BUILDER_H

#include <sc2api/sc2_unit.h>

struct Order;

struct Builder
{
    void onStep();

    bool buildStructure(const sc2::Point2D &starting_point_, Order& order_);

    bool trainUnit(const Order& order_);

private:
    sc2::Units m_freeWorkers;
};

#endif
