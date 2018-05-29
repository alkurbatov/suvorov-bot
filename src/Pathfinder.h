// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_unit.h>

struct Pathfinder {
    // Returns the mineral patch closest to the specified point
    static const sc2::Unit* FindMineralPatch(const sc2::Point2D& point_);

    static sc2::Point2D GetEnemyBaseLocation();
};
