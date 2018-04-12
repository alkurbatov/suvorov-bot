// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Order.h"

#include <sc2api/sc2_unit.h>

struct Builder {
    Builder();

    void OnStep();

    bool Build(Order* order_);

 private:
    sc2::Units m_free_workers;

    int32_t m_minerals;
    int32_t m_vespene;

    float m_available_food;
};
