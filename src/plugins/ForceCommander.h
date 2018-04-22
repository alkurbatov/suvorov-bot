// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Plugin.h"

struct ForceCommander : Plugin {
    void OnStep() final;

    void OnUnitCreated(const sc2::Unit* unit_) final;

 private:
    sc2::Units m_units;
};