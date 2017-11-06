#include "API.h"
#include "Builder.h"
#include "Order.h"
#include "Helpers.h"

void Builder::onStep()
{
    m_freeWorkers = gAPI->observer().getUnits(isFreeWorker());
}

bool Builder::buildStructure(Order& order_)
{
    if (m_freeWorkers.empty())
        return false;

    if (!techRequirementMet(order_))
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

    return true;
}

bool Builder::trainUnit(const Order& order_)
{
    // FIXME: implement assignment of proper producing structure
    if (!order_.m_assignee)
        return false;

    if (!techRequirementMet(order_))
        return false;

    gAPI->action().command(order_);

    return true;
}

bool Builder::techRequirementMet(const Order& order_) const
{
    // Here sc2::UNIT_TYPEID::INVALID means that no tech requirements needed.
    return order_.m_techRequirement == sc2::UNIT_TYPEID::INVALID ||
        gAPI->observer().countUnitType(order_.m_techRequirement) > 0;
}
