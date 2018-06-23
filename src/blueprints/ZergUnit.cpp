// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "../Helpers.h"
#include "ZergUnit.h"

ZergUnit::ZergUnit(): Blueprint(false) {
}

bool ZergUnit::Build(Order* order_) {
    auto larvas = gAPI->observer().GetUnits(IsIdleUnit(sc2::UNIT_TYPEID::ZERG_LARVA));
    if (larvas.empty())
        return false;

    order_->assignee = larvas.front();

    gAPI->action().Build(*order_);

    return true;
}
