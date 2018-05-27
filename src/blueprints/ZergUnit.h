// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Blueprint.h"

struct ZergUnit: Blueprint {
    ZergUnit();

    bool Build(Order* order_) final;
};
