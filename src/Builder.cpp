#include "API.h"
#include "Builder.h"
#include "Order.h"
#include "Helpers.h"

Builder::Builder(): m_minerals(0), m_vespene(0), m_availableFood(0)
{}

void Builder::onStep()
{
    m_freeWorkers = gAPI->observer().getUnits(isFreeWorker());

    m_minerals = gAPI->observer().getMinerals();
    m_vespene = gAPI->observer().getVespene();

    m_availableFood = gAPI->observer().getAvailableFood();
}

bool Builder::buildStructure(Order& order_)
{
    if (m_freeWorkers.empty())
        return false;

    if (!canBuild(order_))
        return false;

    order_.m_assignee = m_freeWorkers.back();
    m_freeWorkers.pop_back();

    // Find place to build the structure
    sc2::Point3D base = gAPI->observer().startingLocation();
    sc2::Point2D point;

    do {
        point.x = base.x + sc2::GetRandomScalar() * 15.0f;
        point.y = base.y + sc2::GetRandomScalar() * 15.0f;
    } while (!gAPI->query().canBePlaced(order_, point));

    gAPI->action().command(order_, point);

    m_minerals -= order_.m_data.mineral_cost;
    m_vespene -= order_.m_data.vespene_cost;

    return true;
}

bool Builder::trainUnit(const Order& order_)
{
    // FIXME: implement assignment of proper producing structure
    if (!order_.m_assignee)
        return false;

    if (!canBuild(order_))
        return false;

    if (m_availableFood < order_.m_data.food_required)
        return false;

    gAPI->action().command(order_);

    m_minerals -= order_.m_data.mineral_cost;
    m_vespene -= order_.m_data.vespene_cost;

    m_availableFood -= order_.m_data.food_required;

    return true;
}

bool Builder::canBuild(const Order& order_) const
{
    if (m_minerals < order_.m_data.mineral_cost || m_vespene < order_.m_data.vespene_cost)
        return false;

    // Here sc2::UNIT_TYPEID::INVALID means that no tech requirements needed.
    return order_.m_data.tech_requirement == sc2::UNIT_TYPEID::INVALID ||
        gAPI->observer().countUnitType(order_.m_data.tech_requirement) > 0;
}
