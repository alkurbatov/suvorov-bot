// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#pragma once

#include "Blueprint.h"

struct Unit: Blueprint {
    explicit Unit(sc2::UNIT_TYPEID who_builds_);

    bool Build(Order* order_) final;

 private:
    sc2::UNIT_TYPEID m_who_builds;
};
