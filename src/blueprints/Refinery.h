// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Blueprint.h"

struct Refinery: Blueprint {
    Refinery();

    virtual bool Build(Order* order_);
};
