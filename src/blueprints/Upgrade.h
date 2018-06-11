// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Blueprint.h"

struct Upgrade: Blueprint {
    Upgrade(): Blueprint(false) {
    }

    virtual bool Build(Order* order_) = 0;
};

struct BuildingUpgrade: Upgrade {
    bool Build(Order* order_) final;
};
