// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Order.h"

#include <sc2api/sc2_unit.h>

struct Miner {
    void OnStep() const;

    std::vector<Order> OnUnitIdle(const sc2::Unit& unit_);
};
