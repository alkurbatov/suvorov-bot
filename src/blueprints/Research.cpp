// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#include "Research.h"
#include "core/API.h"
#include "core/Helpers.h"

Research::Research(sc2::UNIT_TYPEID who_builds_): m_who_builds(who_builds_) {
}

bool Research::Build(Order* order_) {
    auto producers = gAPI->observer().GetUnits(IsIdleUnit(m_who_builds));

    // NOTE (alkurbatov): Queue upgrades if there is no free structures left
    // in order to avoid blocking of the main queue.
    if (producers.Empty())
        producers = gAPI->observer().GetUnits(IsUnit(m_who_builds));

    if (producers.Empty())
        return false;

    order_->assignee = producers().front()->tag;
    gAPI->action().Build(*order_);

    return true;
}
