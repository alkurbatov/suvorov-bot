// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

#include "Building.h"
#include "Hub.h"
#include "core/API.h"

bool HasAddonSpace(Order* order_, const sc2::Point2D& point) {
    if (order_->ability_id != sc2::ABILITY_ID::BUILD_BARRACKS &&
        order_->ability_id != sc2::ABILITY_ID::BUILD_FACTORY &&
        order_->ability_id != sc2::ABILITY_ID::BUILD_STARPORT)
        return true;

    sc2::Point2D addon_pos(point.x + 2.5f, point.y - 0.5f);
    return gAPI->query().CanBePlaced(sc2::ABILITY_ID::BUILD_SUPPLYDEPOT, addon_pos);
}

bool Building::Build(Order* order_) {
    // Find place to build the structure
    sc2::Point3D base = gAPI->observer().StartingLocation();
    sc2::Point2D point;

    unsigned attempt = 0;
    do {
        point.x = base.x + sc2::GetRandomScalar() * 15.0f;
        point.y = base.y + sc2::GetRandomScalar() * 15.0f;

        if (++attempt > 150)
            return false;
    } while (!(gAPI->query().CanBePlaced(*order_, point) &&
               HasAddonSpace(order_, point)));

    return gHub->AssignBuildTask(order_, point);
}
