// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../Hub.h"
#include "TownHall.h"
#include "core/API.h"

bool TownHall::Build(Order* order_) {
    return gHub->AssignBuildTask(order_,
        gHub->GetExpansions().front().town_hall_location);
}
