// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "../Helpers.h"
#include "Upgrade.h"

bool BuildingUpgrade::Build(Order* order_) {
    sc2::Units buildings = gAPI->observer().GetUnits(
        IsUnit(order_->data.tech_alias.back()));
    if (buildings.empty())
        return false;

    for (auto i : buildings) {
        if (!i->orders.empty())
            continue;

        order_->assignee = buildings.front();
        gAPI->action().Build(*order_);

        return true;
    }

    return false;
}
