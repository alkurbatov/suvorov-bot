// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_data.h>
#include <sc2api/sc2_unit.h>

struct Order {
    explicit Order(const sc2::UnitTypeData& data_,
        const sc2::Unit* assignee_ = nullptr):
        data(data_), assignee(assignee_) {
    }

    sc2::UnitTypeData data;
    const sc2::Unit* assignee;
};
