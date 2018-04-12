// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Blueprint.h"

struct Orbital: Blueprint {
    Orbital();

    virtual bool Build(Order* order_);

    virtual bool TechRequirementsMet(const Order& order_);
};
