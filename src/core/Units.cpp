
// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "Units.h"

#include <limits>

Units::Units(const sc2::Units& units_) : m_units(units_) {
}

const sc2::Units& Units::operator()() const {
    return m_units;
}

const sc2::Unit* Units::GetClosestUnit(const sc2::Point2D& point_) const {
    float distance = std::numeric_limits<float>::max();

    const sc2::Unit* target = nullptr;
    for (const auto& i : m_units) {
        float d = sc2::DistanceSquared2D(i->pos, point_);
        if (d < distance) {
            distance = d;
            target = i;
        }
    }

    return target;
}
