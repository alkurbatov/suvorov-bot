// The MIT License (MIT)
//
// Copyright (c) 2017-2022 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_unit.h>

struct Units {
    explicit Units(const sc2::Units& units_);

    bool Empty() const;

    const sc2::Units& operator()() const;

    const sc2::Unit* GetClosestUnit(const sc2::Point2D& point_) const;

    const sc2::Unit* GetClosestUnit(sc2::Tag tag_) const;

    const sc2::Unit* GetRandomUnit() const;

    bool HasOrder(sc2::ABILITY_ID id_) const;

 private:
    sc2::Units m_units;
};
