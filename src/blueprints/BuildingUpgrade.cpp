// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "BuildingUpgrade.h"
#include "core/API.h"
#include "core/Helpers.h"

bool BuildingUpgrade::Build(Order* order_) {
    sc2::Units buildings = gAPI->observer().GetUnits(
        IsIdleUnit(order_->tech_alias.back()));

    if (buildings.empty())
        return false;

    order_->assignee = buildings.front();
    gAPI->action().Build(*order_);

    return true;
}
