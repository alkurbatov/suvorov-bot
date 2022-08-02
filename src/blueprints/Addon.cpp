// The MIT License (MIT)
//
// Copyright (c) 2017-2022 Alexander Kurbatov

#include "Addon.h"
#include "core/API.h"
#include "core/Helpers.h"

Addon::Addon(sc2::UNIT_TYPEID who_builds_): m_who_builds(who_builds_) {
}

bool Addon::Build(Order* order_) {
    auto producers = gAPI->observer().GetUnits(IsIdleUnit(m_who_builds));
    if (producers.Empty())
        return false;

    for (const auto& i : producers()) {
        if (i->add_on_tag)
            continue;

        // NOTE (alkurbatov): Size of addon is equal to size of supply depot.
        sc2::Point2D addon_pos(i->pos.x + 2.5f, i->pos.y - 0.5f);
        if (!gAPI->query().CanBePlaced(sc2::ABILITY_ID::BUILD_SUPPLYDEPOT, addon_pos))
            continue;

        order_->assignee = i->tag;
        gAPI->action().Build(*order_);
        return true;
    }

    // FIXME (alkurbatov): We should either reposition barracks/factory/starport to have
    // some place for addons or reserve place for addon in some way.
    return false;
}
