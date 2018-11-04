// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_unit.h>

struct Units {
    explicit Units(const sc2::Units& units_);

    const sc2::Units& operator()() const;

    const sc2::Unit* GetClosestUnit(const sc2::Point2D& point_) const;

 private:
    sc2::Units m_units;
};
