// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Builder.h"
#include "Plugin.h"

struct QuarterMaster : Plugin {
    QuarterMaster();

    void OnStep(Builder* builder_) final;

    void OnUnitCreated(const sc2::Unit* unit_) final;

 private:
    bool m_skip_turn;
};
