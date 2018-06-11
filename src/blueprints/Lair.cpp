// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "Lair.h"
#include "../Helpers.h"

Lair::Lair(): Blueprint(false) {
}

bool Lair::Build(Order* order_) {
    sc2::Units hatcheries = gAPI->observer().GetUnits(
        IsUnit(sc2::UNIT_TYPEID::ZERG_HATCHERY));
    if (hatcheries.empty())
        return false;

    order_->assignee = hatcheries.front();

    gAPI->action().Build(*order_);

    return true;
}
