#pragma once

#include "Order.h"

#include <sc2api/sc2_unit.h>

struct Builder {
    Builder();

    void OnStep();

    bool BuildStructure(Order* order_);

    bool BuildRefinery(Order* order_);

    bool TrainUnit(const Order& order_);

private:
    bool CanBuild(const Order& order_) const;

    sc2::Units m_free_workers;

    int32_t m_minerals;
    int32_t m_vespene;

    float m_available_food;
};
