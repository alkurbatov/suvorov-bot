// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Order.h"

#include <list>

struct Diagnosis {
    void OnStep() const;

    void ShowBuildOrder(const std::list<Order>& orders_) const;
};
