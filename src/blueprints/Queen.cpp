// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#include "Queen.h"
#include "core/API.h"
#include "core/Helpers.h"

bool Queen::Build(Order* order_) {
    auto town_halls = gAPI->observer().GetUnits(IsIdleTownHall());
    if (town_halls.Empty())
        return false;

    order_->assignee = town_halls().front()->tag;

    gAPI->action().Build(*order_);

    return true;
}
