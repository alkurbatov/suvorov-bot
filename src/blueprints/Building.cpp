// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "Building.h"

Building::Building(): Blueprint(true) {
}

bool Building::Build(Order* order_) {
    // Find place to build the structure
    sc2::Point3D base = gAPI->observer().StartingLocation();
    sc2::Point2D point;

    do {
        point.x = base.x + sc2::GetRandomScalar() * 15.0f;
        point.y = base.y + sc2::GetRandomScalar() * 15.0f;
    } while (!gAPI->query().CanBePlaced(*order_, point));

    gAPI->action().Command(*order_, point);

    return true;
}
