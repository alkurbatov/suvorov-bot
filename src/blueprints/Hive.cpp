// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "Hive.h"
#include "../Helpers.h"

Hive::Hive(): Blueprint(false) {
}

bool Hive::Build(Order* order_) {
    sc2::Units lairs = gAPI->observer().GetUnits(
        IsUnit(sc2::UNIT_TYPEID::ZERG_LAIR));
    if (lairs.empty())
        return false;

    order_->assignee = lairs.front();

    gAPI->action().Build(*order_);

    return true;
}
