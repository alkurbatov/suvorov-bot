#include "API.h"
#include "Builder.h"
#include "Order.h"
#include "Helpers.h"

bool Builder::buildStructure(const sc2::Point2D& starting_point_, Order& order_)
{
    auto workers = gAPI->observer().getOwnUnits(isFreeWorker());
    if (workers.empty())
        return false;

    order_.m_assignee = workers.front();

    // Find place to build the structure
    sc2::Point2D point;
    do {
        point.x = starting_point_.x + sc2::GetRandomScalar() * 15.0f;
        point.y = starting_point_.y + sc2::GetRandomScalar() * 15.0f;
    } while (!gAPI->query().canBePlaced(order_, point));

    gAPI->action().command(order_, point);

    return true;
}

bool Builder::trainUnit(const Order& order_)
{
    // FIXME: implement assignment of proper producing structure
    if (!order_.m_assignee)
        return false;

    gAPI->action().command(order_);

    return true;
}
