// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "blueprints/Blueprint.h"
#include "Builder.h"
#include "Converter.h"
#include "Helpers.h"
#include "Historican.h"
#include "Pathfinder.h"

#include <algorithm>
#include <memory>

Builder::Builder(): m_minerals(0), m_vespene(0), m_available_food(0.0f) {
}

void Builder::OnStep() {
    m_free_workers = gAPI->observer().GetUnits(IsFreeWorker());

    m_minerals = gAPI->observer().GetMinerals();
    m_vespene = gAPI->observer().GetVespene();

    m_available_food = gAPI->observer().GetAvailableFood();

    auto it = m_construction_orders.begin();
    while (it != m_construction_orders.end()) {
        if (!Build(&(*it)))
            break;

        it = m_construction_orders.erase(it);
    }

    it = m_training_orders.begin();
    while (it != m_training_orders.end()) {
        if (!Build(&(*it))) {
            ++it;
            continue;
        }

        it = m_training_orders.erase(it);
    }
}

void Builder::ScheduleConstruction(sc2::UNIT_TYPEID id_) {
    auto& data = gAPI->observer().GetUnitTypeData();

    sc2::UnitTypeData structure = data[convert::ToUnitTypeID(id_)];

    // NOTE(alkurbatov): Unfortunally SC2 API returns wrong mineral cost
    // and tech_requirement for orbital command and planetary fortress
    // so we use a workaround.
    // See https://github.com/Blizzard/s2client-api/issues/191
    if (id_ == sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND) {
        structure.mineral_cost = 150;
        structure.tech_requirement = sc2::UNIT_TYPEID::TERRAN_BARRACKS;
    } else if (id_ == sc2::UNIT_TYPEID::TERRAN_PLANETARYFORTRESS) {
        structure.mineral_cost = 150;
        structure.tech_requirement = sc2::UNIT_TYPEID::TERRAN_ENGINEERINGBAY;
    }

    // Prevent deadlock.
    if (structure.tech_requirement != sc2::UNIT_TYPEID::INVALID &&
        gAPI->observer().CountUnitType(structure.tech_requirement) == 0 &&
        CountScheduledStructures(structure.tech_requirement) == 0) {
            ScheduleConstruction(structure.tech_requirement);
    }

    m_construction_orders.emplace_back(structure);
}

void Builder::ScheduleTraining(sc2::UNIT_TYPEID id_, const sc2::Unit* unit_) {
    auto& data = gAPI->observer().GetUnitTypeData();

    m_training_orders.emplace_back(data[
        convert::ToUnitTypeID(id_)], unit_);
}

const std::list<Order>& Builder::GetConstructionOrders() const {
    return m_construction_orders;
}

int64_t Builder::CountScheduledStructures(sc2::UNIT_TYPEID id_) const {
    return std::count_if(
        m_construction_orders.begin(),
        m_construction_orders.end(),
        IsOrdered(id_));
}

bool Builder::Build(Order* order_) {
    if (m_minerals < order_->data.mineral_cost || m_vespene < order_->data.vespene_cost)
        return false;

    std::shared_ptr<Blueprint> blueprint = Blueprint::Plot(order_->data.ability_id);

    // Here sc2::UNIT_TYPEID::INVALID means that no tech requirements needed.
    if (order_->data.tech_requirement != sc2::UNIT_TYPEID::INVALID &&
        gAPI->observer().CountUnitType(order_->data.tech_requirement) == 0) {
            return false;
    }

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
