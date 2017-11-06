#ifndef BUILDER_H
#define BUILDER_H

#include <sc2api/sc2_unit.h>

struct Order;

struct Builder
{
    void onStep();

    bool buildStructure(Order& order_);

    bool trainUnit(const Order& order_);

private:
    bool techRequirementMet(const Order& order_) const;

    sc2::Units m_freeWorkers;
};

#endif
