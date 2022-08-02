// The MIT License (MIT)
//
// Copyright (c) 2017-2022 Alexander Kurbatov

#pragma once

#include "Builder.h"
#include "strategies/Strategy.h"

struct ZerglingFlood : Strategy {
    ZerglingFlood();

    void OnGameStart(Builder* builder_) final;

    void OnUnitIdle(const sc2::Unit* unit_, Builder* builder_) final;
};
