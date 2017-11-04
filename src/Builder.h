#ifndef BUILDER_H
#define BUILDER_H

#include <list>
#include <boost/core/noncopyable.hpp>

struct Order;

class Builder: boost::noncopyable
{
public:
    Builder(sc2::ActionInterface* action_, const sc2::ObservationInterface* observation_,
        sc2::QueryInterface* query_);

    bool execute(const sc2::Point2D &starting_point_, const Order& order_);

private:
    sc2::ActionInterface* m_action;
    const sc2::ObservationInterface* m_observation;
    sc2::QueryInterface* m_query;
};

#endif
