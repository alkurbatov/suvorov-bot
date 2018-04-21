// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "Fortress.h"
#include "../Helpers.h"

Fortress::Fortress(): Blueprint(false) {
}

bool Fortress::Build(Order* order_) {
    sc2::Units command_centers = gAPI->observer().GetUnits(IsFreeCommandCenter());
    if (command_centers.empty())
        return false;

    order_->assignee = command_centers.front();

    gAPI->action().Build(*order_);

    return true;
}
