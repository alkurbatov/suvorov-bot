// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#include "Hub.h"
#include "TownHall.h"
#include "core/API.h"

bool TownHall::Build(Order* order_) {
    const sc2::Point3D* town_hall_location = gHub->GetNextExpansion();
    if (!town_hall_location)
        return false;

    return gHub->AssignBuildTask(order_, *town_hall_location);
}
