// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

#pragma once

#include "Blueprint.h"

struct TownHall: Blueprint {
    bool Build(Order* order_) final;
};
