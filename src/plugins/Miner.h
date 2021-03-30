// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#pragma once

#include "Builder.h"
#include "Plugin.h"

struct Miner : Plugin {
    Miner();

    void OnStep(Builder* builder_) final;

    void OnUnitCreated(const sc2::Unit* unit_, Builder*) final;

    void OnUnitIdle(const sc2::Unit* unit_, Builder*) final;
};
