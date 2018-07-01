// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "Pathfinder.h"
#include "core/API.h"
#include "core/Helpers.h"

#include <sc2api/sc2_map_info.h>
#include <sc2api/sc2_typeenums.h>

const sc2::Unit* Pathfinder::FindMineralPatch(const sc2::Point2D& point_) {
    return gAPI->observer().GetClosestUnit(point_, IsMineralPatch(),
        sc2::Unit::Alliance::Neutral);
}

sc2::Point2D Pathfinder::GetEnemyBaseLocation() {
    // FIXME(alkurbatov): We must scout first, there could be several possible locations.
    return gAPI->observer().GameInfo().enemy_start_locations.front();
}
