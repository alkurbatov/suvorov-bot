// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "Mutation.h"
#include "core/API.h"
#include "core/Helpers.h"

bool Mutation::Build(Order* order_) {
    sc2::Units targets = gAPI->observer().GetUnits(
        IsIdleUnit(order_->tech_alias.back()));

    if (targets.empty())
        return false;

    order_->assignee = targets.front();
    gAPI->action().Build(*order_);

    return true;
}
