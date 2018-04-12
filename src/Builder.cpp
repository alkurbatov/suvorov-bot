// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "blueprints/Blueprint.h"
#include "Builder.h"
#include "Helpers.h"
#include "Historican.h"
#include "Pathfinder.h"

#include <memory>

Builder::Builder(): m_minerals(0), m_vespene(0), m_available_food(0.0f) {
}

void Builder::OnStep() {
    m_free_workers = gAPI->observer().GetUnits(IsFreeWorker());

    m_minerals = gAPI->observer().GetMinerals();
    m_vespene = gAPI->observer().GetVespene();

    m_available_food = gAPI->observer().GetAvailableFood();
}

bool Builder::Build(Order* order_) {
    if (m_minerals < order_->data.mineral_cost || m_vespene < order_->data.vespene_cost)
        return false;

    std::shared_ptr<Blueprint> blueprint = Blueprint::Plot(order_->data.ability_id);

    if (!blueprint->TechRequirementsMet(*order_))
        return false;

    if (m_available_food < order_->data.food_required)
        return false;

    if (blueprint->NeedsWorker()) {
        if (m_free_workers.empty())
            return false;

        order_->assignee = m_free_workers.back();
        m_free_workers.pop_back();
    }

    if (!blueprint->Build(order_))
        return false;

    m_minerals -= order_->data.mineral_cost;
    m_vespene -= order_->data.vespene_cost;
    m_available_food -= order_->data.food_required;

    gHistory << "Started building a " << order_->data.name << std::endl;
    return true;
}
