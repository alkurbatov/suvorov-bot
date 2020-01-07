// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

#pragma once

#include "core/Order.h"

#include <list>

#include <sc2api/sc2_typeenums.h>
#include <sc2api/sc2_unit.h>

struct Builder {
    Builder();

    void OnStep();

    void OnUnitCreated(const sc2::Unit& unit_);

    void ScheduleConstruction(sc2::UNIT_TYPEID id_, bool urgent = false);

    void ScheduleUpgrade(sc2::UPGRADE_ID id_);

    void ScheduleTraining(sc2::UNIT_TYPEID id_,
        const sc2::Unit* unit_ = nullptr, bool urgent = false);

    void ScheduleOrders(const std::vector<Order>& orders_);

    const std::list<Order>& GetConstructionOrders() const;

    const std::list<Order>& GetTrainingOrders() const;

    int64_t CountScheduledStructures(sc2::UNIT_TYPEID id_) const;

    int64_t CountScheduledTrainings(sc2::UNIT_TYPEID id_) const;

 private:
    bool Build(Order* order_);

    uint32_t m_minerals;
    uint32_t m_vespene;

    float m_available_food;

    std::list<Order> m_construction_orders;
    std::list<Order> m_training_orders;
};
