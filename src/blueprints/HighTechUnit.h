// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

#pragma once

#include "Blueprint.h"

struct HighTechUnit: Blueprint {
    HighTechUnit(sc2::UNIT_TYPEID who_builds_, sc2::UNIT_TYPEID addon_type_);

    bool Build(Order* order_) final;

 private:
    sc2::UNIT_TYPEID m_who_builds;
    sc2::UNIT_TYPEID m_addon_type;
};
