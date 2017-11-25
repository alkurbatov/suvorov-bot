// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_unit.h>

struct ForceCommander {
    ForceCommander();

    void OnStep();

    void OnUnitCreated(const sc2::Unit* unit_);

 private:
    sc2::Units m_units;
    bool m_attack_started;
};
