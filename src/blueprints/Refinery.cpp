// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../Hub.h"
#include "Refinery.h"
#include "core/API.h"
#include "core/Helpers.h"

bool Refinery::Build(Order* order_) {
    sc2::Point3D base = gAPI->observer().StartingLocation();

    auto geyser = gAPI->observer().GetClosestUnit(base, IsFreeGeyser(),
        sc2::Unit::Alliance::Neutral);

    if (!geyser)
        return false;

    return gHub->AssignRefineryConstruction(order_, geyser);
}
