// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "../Helpers.h"
#include "Orbital.h"

Orbital::Orbital(): Blueprint(false) {
}

bool Orbital::Build(Order* order_) {
    sc2::Units command_centers = gAPI->observer().GetUnits(IsFreeCommandCenter());
    if (command_centers.empty())
        return false;

    order_->assignee = command_centers.front();

    gAPI->action().Command(*order_);

    return true;
}
