// The MIT License (MIT)
//
// Copyright (c) 2017-2022 Alexander Kurbatov

#pragma once

#include "plugins/Plugin.h"

struct Strategy : Plugin {
    explicit Strategy(float attack_limit_);

    void OnStep(Builder*) override;

    void OnUnitCreated(const sc2::Unit* unit_, Builder*) override;

 protected:
    float m_attack_limit;
    sc2::Units m_units;
};
