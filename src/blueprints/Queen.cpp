// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "../Helpers.h"
#include "Queen.h"

Queen::Queen(): Blueprint(false) {
}

bool Queen::Build(Order* order_) {
    auto town_halls = gAPI->observer().GetUnits(IsIdleTownHall());
    if (town_halls.empty())
        return false;

    order_->assignee = town_halls.front();

    gAPI->action().Build(*order_);

    return true;
}
