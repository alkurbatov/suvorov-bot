#include "API.h"
#include "Builder.h"
#include "Helpers.h"

Builder::Builder(): m_minerals(0), m_vespene(0), m_available_food(0.0f) {
}

void Builder::OnStep() {
    m_free_workers = gAPI->observer().GetUnits(IsFreeWorker());

    m_minerals = gAPI->observer().GetMinerals();
    m_vespene = gAPI->observer().GetVespene();

    m_available_food = gAPI->observer().GetAvailableFood();
}

bool Builder::BuildStructure(Order* order_) {
    if (m_free_workers.empty())
        return false;

    if (!CanBuild(*order_))
        return false;

    order_->assignee = m_free_workers.back();
    m_free_workers.pop_back();

    // Find place to build the structure
    sc2::Point3D base = gAPI->observer().StartingLocation();
    sc2::Point2D point;

    do {
        point.x = base.x + sc2::GetRandomScalar() * 15.0f;
        point.y = base.y + sc2::GetRandomScalar() * 15.0f;
    } while (!gAPI->query().CanBePlaced(*order_, point));

    gAPI->action().Command(*order_, point);

    m_minerals -= order_->data.mineral_cost;
    m_vespene -= order_->data.vespene_cost;

    return true;
}

bool Builder::TrainUnit(const Order& order_) {
    // FIXME: implement assignment of proper producing structure
    if (!order_.assignee)
        return false;

    if (!CanBuild(order_))
        return false;

    if (m_available_food < order_.data.food_required)
        return false;

    gAPI->action().Command(order_);

    m_minerals -= order_.data.mineral_cost;
    m_vespene -= order_.data.vespene_cost;

    m_available_food -= order_.data.food_required;

    return true;
}

bool Builder::CanBuild(const Order& order_) const {
    if (m_minerals < order_.data.mineral_cost || m_vespene < order_.data.vespene_cost)
        return false;

    // Here sc2::UNIT_TYPEID::INVALID means that no tech requirements needed.
    return order_.data.tech_requirement == sc2::UNIT_TYPEID::INVALID ||
        gAPI->observer().CountUnitType(order_.data.tech_requirement) > 0;
}
