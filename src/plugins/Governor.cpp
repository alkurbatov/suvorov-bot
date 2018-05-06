// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "Governor.h"

#include <numeric>

Governor::Governor(std::shared_ptr<Builder> builder_): Plugin(), m_builder(builder_) {
}

void Governor::OnStep() {
    auto builder = m_builder.lock();
    if (!builder)
        return;

    float expected_food = API::FOOD_PROVIDED::TERRAN_SUPPLY
        * builder->CountScheduledStructures(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT)
        + gAPI->observer().GetExpectedFoodCap();

    const std::list<Order> training_orders = builder->GetTrainigOrders();
    float expected_consumption = gAPI->observer().GetFoodUsed()
        + API::FOOD_PROVIDED::TERRAN_SUPPLY
        + std::accumulate(
            training_orders.begin(),
            training_orders.end(),
            0, [](int food, const Order& order_) {
                return food + order_.data.food_required;
            });

    if (expected_food > expected_consumption)
        return;

    builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT, true);
}
