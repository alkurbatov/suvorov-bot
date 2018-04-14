// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Order.h"

#include <list>

#include <sc2api/sc2_typeenums.h>
#include <sc2api/sc2_unit.h>

struct Builder {
    Builder();

    void OnStep();

    void ScheduleConstruction(sc2::UNIT_TYPEID id_);

    void ScheduleTraining(sc2::UNIT_TYPEID id_, const sc2::Unit* unit_);

    const std::list<Order>& GetConstructionOrders() const;

 private:
    bool Build(Order* order_);

    sc2::Units m_free_workers;

    int32_t m_minerals;
    int32_t m_vespene;

    float m_available_food;

    std::list<Order> m_construction_orders;
    std::list<Order> m_training_orders;
};
