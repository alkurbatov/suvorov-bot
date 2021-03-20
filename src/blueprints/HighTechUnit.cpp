// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#include "HighTechUnit.h"
#include "core/API.h"
#include "core/Helpers.h"

HighTechUnit::HighTechUnit(sc2::UNIT_TYPEID who_builds_, sc2::UNIT_TYPEID addon_type_):
    m_who_builds(who_builds_), m_addon_type(addon_type_) {
}

bool HighTechUnit::Build(Order* order_) {
    if (order_->assignee) {
        gAPI->action().Build(*order_);
        return true;
    }

    Units producers = gAPI->observer().GetUnits(IsIdleUnit(m_who_builds));
    for (const auto& i : producers()) {
        if (i->add_on_tag == 0)
            continue;

        const sc2::Unit* addon = gAPI->observer().GetUnit(i->add_on_tag);
        if (!addon || addon->unit_type != m_addon_type)
            continue;

        order_->assignee = i->tag;
        gAPI->action().Build(*order_);
        return true;
    }

    return false;
}
