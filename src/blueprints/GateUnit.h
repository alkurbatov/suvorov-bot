// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#pragma once

#include "Blueprint.h"

struct GateUnit: Blueprint {
    bool Build(Order* order_) final;
};
