// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#include "Hub.h"
#include "TownHall.h"
#include "core/API.h"

bool TownHall::Build(Order* order_) {
    Expansion* next_expand = gHub->GetNextExpansion();
    if (!next_expand)
        return false;

    sc2::Tag builder = gHub->AssignBuildTask(order_, next_expand->town_hall_location);
    if (!builder)
        return false;

    next_expand->worker_tag = builder;

    return true;
}
