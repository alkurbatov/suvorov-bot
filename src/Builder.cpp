#include <sc2api/sc2_interfaces.h>

#include "Builder.h"
#include "Order.h"
#include "Helpers.h"

Builder::Builder(sc2::ActionInterface* action_,
    const sc2::ObservationInterface* observation_,
    sc2::QueryInterface* query_):
    m_action(action_), m_observation(observation_), m_query(query_)
{}

bool Builder::execute(const sc2::Point2D& starting_point_, const Order& order_)
{
    if (order_.m_assignee) {
        m_action->UnitCommand(order_.m_assignee, order_.m_id);
        return true;
    }

    auto workers = m_observation->GetUnits(sc2::Unit::Alliance::Self, isFreeWorker());
    if (workers.empty())
        return false;

    // Find place to build the structure
    sc2::Point2D point;
    do {
        point.x = starting_point_.x + sc2::GetRandomScalar() * 15.0f;
        point.y = starting_point_.y + sc2::GetRandomScalar() * 15.0f;
    } while (!m_query->Placement(order_.m_id, point));

    m_action->UnitCommand(workers.front(), order_.m_id, point);

    return true;
}
