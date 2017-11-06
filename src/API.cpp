#include "API.h"
#include "Order.h"

namespace API
{

Action::Action(sc2::ActionInterface* action_): m_action(action_)
{
}

void Action::command(const Order& order_)
{
    m_action->UnitCommand(order_.m_assignee, order_.m_id);
}

void Action::command(const Order& order_, const sc2::Point2D& point_)
{
    m_action->UnitCommand(order_.m_assignee, order_.m_id, point_);
}

Observer::Observer(const sc2::ObservationInterface* observer_):
    m_observer(observer_)
{
}

sc2::Units Observer::getOwnUnits(const sc2::Filter& filter_)
{
    return m_observer->GetUnits(sc2::Unit::Alliance::Self, filter_);
}

Query::Query(sc2::QueryInterface* query_): m_query(query_)
{
}

bool Query::canBePlaced(const Order& order_, const sc2::Point2D& point_)
{
    return m_query->Placement(order_.m_id, point_);
}

Interface::Interface(sc2::ActionInterface* action_,
    const sc2::ObservationInterface* observer_, sc2::QueryInterface* query_):
    m_action(action_), m_observer(observer_), m_query(query_)
{}

Action Interface::action() const
{
    return Action(m_action);
}

Observer Interface::observer() const
{
    return Observer(m_observer);
}

Query Interface::query() const
{
    return Query(m_query);
}

} //namespace API

std::unique_ptr<API::Interface> gAPI;
