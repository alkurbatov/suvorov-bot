// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "../Helpers.h"
#include "../World.h"
#include "Refinery.h"

Refinery::Refinery(): Blueprint(true) {
}

bool Refinery::Build(Order* order_) {
    sc2::Point3D base = gAPI->observer().StartingLocation();

    auto geyser = gAPI->observer().GetClosestUnit(base, IsFreeGeyser(),
        sc2::Unit::Alliance::Neutral);

    if (!geyser)
        return false;

    gAPI->action().Build(*order_, geyser);
    gWorld->ClaimObject(*geyser);

    return true;
}
