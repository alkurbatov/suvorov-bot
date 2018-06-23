// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "../Helpers.h"
#include "Upgrade.h"

bool BuildingUpgrade::Build(Order* order_) {
    sc2::Units buildings = gAPI->observer().GetUnits(
        IsIdleUnit(order_->data.tech_alias.back()));

    if (buildings.empty())
        return false;

    order_->assignee = buildings.front();
    gAPI->action().Build(*order_);

    return true;
}
