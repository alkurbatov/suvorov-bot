// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#pragma once

#include "GameObject.h"

#include <sc2api/sc2_unit.h>

struct Geyser: GameObject {
    explicit Geyser(const sc2::Unit& unit_);

    explicit Geyser(const sc2::UnitOrder& order_);

    bool operator==(const Geyser& geyser_) const;

 private:
    sc2::Point2D m_pos;
};
