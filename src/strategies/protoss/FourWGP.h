// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#pragma once

#include "Builder.h"
#include "strategies/Strategy.h"

struct FourWGP : Strategy {
    FourWGP();

    void OnGameStart(Builder* builder_) final;

    void OnUnitIdle(const sc2::Unit* unit_, Builder* builder_) final;
};
