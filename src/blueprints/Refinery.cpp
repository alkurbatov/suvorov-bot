// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "../Pathfinder.h"
#include "Refinery.h"

Refinery::Refinery(): Blueprint(true) {
}

bool Refinery::Build(Order* order_) {
    sc2::Point3D base = gAPI->observer().StartingLocation();

    const sc2::Unit* geiser = Pathfinder::FindVespeneGeyser(base);
    if (!geiser)
        return false;

    gAPI->action().Build(*order_, geiser);

    return true;
}
