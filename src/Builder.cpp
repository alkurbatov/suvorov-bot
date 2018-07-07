// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "blueprints/Blueprint.h"
#include "core/API.h"
#include "core/Helpers.h"
#include "objects/Worker.h"
#include "Builder.h"
#include "Historican.h"
#include "Hub.h"
#include "Pathfinder.h"

#include <algorithm>
#include <memory>

Builder::Builder(): m_minerals(0), m_vespene(0),
    m_reserved_minerals(0), m_reserved_vespene(0), m_available_food(0.0f) {
}

void Builder::OnStep() {
    m_minerals = gAPI->observer().GetMinerals() - m_reserved_minerals;
    m_vespene = gAPI->observer().GetVespene() - m_reserved_vespene;

    m_available_food = gAPI->observer().GetAvailableFood();

    auto it = m_construction_orders.begin();
    while (it != m_construction_orders.end()) {
        if (!Build(&(*it)))
            break;

        if (it->data.tech_alias.empty()) {  // Skip building upgrades.
            m_reserved_minerals += it->data.mineral_cost;
            m_reserved_vespene += it->data.vespene_cost;
        }

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

void Builder::OnUnitCreated(const sc2::Unit& unit_) {
    if (unit_.build_progress > 0.0f)
        return;

    sc2::UnitTypeData data = gAPI->observer().GetUnitTypeData(unit_.unit_type);

    gHistory << "[INFO] Decreasing reserved resources: -" <<
        data.mineral_cost << " minerals, -" <<
        data.vespene_cost << " vespene" << std::endl;

    m_reserved_minerals -= data.mineral_cost;
    m_reserved_vespene -= data.vespene_cost;

    gHistory << "[INFO] Reserved minerals left: " << m_reserved_minerals << std::endl;
    gHistory << "[INFO] Reserved vespene left: " << m_reserved_vespene << std::endl;
}

void Builder::ScheduleConstruction(sc2::UNIT_TYPEID id_, bool urgent) {
    sc2::UnitTypeData structure = gAPI->observer().GetUnitTypeData(id_);

    // Prevent deadlock.
    if (structure.tech_requirement != sc2::UNIT_TYPEID::INVALID &&
        gAPI->observer().CountUnitType(structure.tech_requirement) == 0 &&
        CountScheduledStructures(structure.tech_requirement) == 0) {
            ScheduleConstruction(structure.tech_requirement);
    }

    if (urgent) {
        m_construction_orders.emplace_front(structure);
        return;
    }

    m_construction_orders.emplace_back(structure);
}

void Builder::ScheduleTraining(sc2::UNIT_TYPEID id_,
    const sc2::Unit* unit_, bool urgent) {
    if (urgent) {
        m_training_orders.emplace_front(gAPI->observer().GetUnitTypeData(id_), unit_);
        return;
    }

    m_training_orders.emplace_back(gAPI->observer().GetUnitTypeData(id_), unit_);
}

void Builder::ScheduleOrders(const std::vector<Order>& orders_) {
    // FIXME (alkurbatov): this call must be more intellectual
    // and able to select a proper queue.
    for (const auto& i : orders_)
        m_training_orders.emplace_back(i);
}

const std::list<Order>& Builder::GetConstructionOrders() const {
    return m_construction_orders;
}

const std::list<Order>& Builder::GetTrainigOrders() const {
    return m_training_orders;
}

int64_t Builder::CountScheduledStructures(sc2::UNIT_TYPEID id_) const {
    return std::count_if(
        m_construction_orders.begin(),
        m_construction_orders.end(),
        IsOrdered(id_));
}

int64_t Builder::CountScheduledTrainings(sc2::UNIT_TYPEID id_) const {
    return std::count_if(
        m_training_orders.begin(),
        m_training_orders.end(),
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

    if (!blueprint->Build(order_))
        return false;

    m_minerals -= order_->data.mineral_cost;
    m_vespene -= order_->data.vespene_cost;
    m_available_food -= order_->data.food_required;

    gHistory << "[INFO] Started building a " << order_->data.name << std::endl;
    return true;
}
