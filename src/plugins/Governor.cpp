// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "../World.h"
#include "Governor.h"

#include <numeric>

Governor::Governor(std::shared_ptr<Builder> builder_): Plugin(), m_builder(builder_) {
}

void Governor::OnGameStart() {
    auto builder = m_builder.lock();
    if (!builder)
        return;

    // Initial build order
    switch (gWorld->GetCurrentRace()) {
        case sc2::Race::Terran:
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_REFINERY);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
            return;

        case sc2::Race::Zerg:
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_OVERLORD);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_SPAWNINGPOOL);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_EXTRACTOR);
            builder->ScheduleConstruction(sc2::UNIT_TYPEID::ZERG_OVERLORD);
            return;

        default:
            return;
    }
}

void Governor::OnStep() {
    if (gWorld->GetCurrentRace() != sc2::Race::Terran)
        return;

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

void Governor::OnUnitIdle(const sc2::Unit* unit_) {
    auto builder = m_builder.lock();
    if (!builder)
        return;

    if (unit_->unit_type.ToType() == sc2::UNIT_TYPEID::TERRAN_BARRACKS)
        builder->ScheduleTraining(sc2::UNIT_TYPEID::TERRAN_MARINE, unit_);

    if (unit_->unit_type.ToType() == sc2::UNIT_TYPEID::ZERG_LARVA)
        builder->ScheduleTraining(sc2::UNIT_TYPEID::ZERG_ZERGLING, unit_);
}
