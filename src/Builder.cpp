// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "blueprints/Blueprint.h"
#include "Builder.h"
#include "Helpers.h"
#include "Historican.h"
#include "Pathfinder.h"
#include "World.h"

#include <algorithm>
#include <memory>

Builder::Builder(): m_minerals(0), m_vespene(0), m_available_food(0.0f) {
}

void Builder::OnStep() {
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

void Builder::ScheduleConstruction(sc2::UNIT_TYPEID id_, bool urgent) {
    sc2::UnitTypeData structure = gAPI->observer().GetUnitTypeData(id_);

    switch (id_) {
        // NOTE (alkurbatov): Unfortunally SC2 API returns wrong mineral cost
        // and tech_requirement for orbital command, planetary fortress,
        // lair, hive and greater spire.
        // so we use a workaround.
        // See https://github.com/Blizzard/s2client-api/issues/191
        case sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND:
            structure.mineral_cost = 150;
            structure.tech_requirement = sc2::UNIT_TYPEID::TERRAN_BARRACKS;
            break;

        case sc2::UNIT_TYPEID::ZERG_GREATERSPIRE:
            structure.mineral_cost = 100;
            structure.vespene_cost = 150;
            structure.tech_requirement = sc2::UNIT_TYPEID::ZERG_HIVE;
            break;

        case sc2::UNIT_TYPEID::TERRAN_PLANETARYFORTRESS:
            structure.mineral_cost = 150;
            structure.tech_requirement = sc2::UNIT_TYPEID::TERRAN_ENGINEERINGBAY;
            break;

        case sc2::UNIT_TYPEID::ZERG_LAIR:
            structure.mineral_cost = 150;
            structure.tech_requirement = sc2::UNIT_TYPEID::ZERG_SPAWNINGPOOL;
            break;

        case sc2::UNIT_TYPEID::ZERG_HIVE:
            structure.mineral_cost = 200;
            structure.vespene_cost = 150;
            structure.tech_requirement = sc2::UNIT_TYPEID::ZERG_INFESTATIONPIT;
            break;

        // NOTE (alkurbatov): By some reason all zerg buildings
        // include drone mineral cost.
        case sc2::UNIT_TYPEID::ZERG_BANELINGNEST:
        case sc2::UNIT_TYPEID::ZERG_EVOLUTIONCHAMBER:
        case sc2::UNIT_TYPEID::ZERG_EXTRACTOR:
        case sc2::UNIT_TYPEID::ZERG_INFESTATIONPIT:
        case sc2::UNIT_TYPEID::ZERG_HYDRALISKDEN:
        case sc2::UNIT_TYPEID::ZERG_ROACHWARREN:
        case sc2::UNIT_TYPEID::ZERG_SPAWNINGPOOL:
        case sc2::UNIT_TYPEID::ZERG_SPINECRAWLER:
        case sc2::UNIT_TYPEID::ZERG_SPIRE:
        case sc2::UNIT_TYPEID::ZERG_SPORECRAWLER:
        case sc2::UNIT_TYPEID::ZERG_ULTRALISKCAVERN:
            structure.mineral_cost -= 50;
            break;

        // NOTE (alkurbatov): There is no sense in summoning protoss buildings
        // without a pylon.
        case sc2::UNIT_TYPEID::PROTOSS_FORGE:
        case sc2::UNIT_TYPEID::PROTOSS_GATEWAY:
            structure.tech_requirement = sc2::UNIT_TYPEID::PROTOSS_PYLON;
            break;

        default:
            break;
    }

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

void Builder::ScheduleTraining(sc2::UNIT_TYPEID id_, const sc2::Unit* unit_) {
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

    if (blueprint->NeedsWorker()) {
        const sc2::Unit* worker = gWorld->GetFreeWorker();
        if (!worker)
            return false;

        order_->assignee = worker;
    }

    if (!blueprint->Build(order_))
        return false;

    m_minerals -= order_->data.mineral_cost;
    m_vespene -= order_->data.vespene_cost;
    m_available_food -= order_->data.food_required;

    gHistory << "[INFO] Started building a " << order_->data.name << std::endl;
    return true;
}
