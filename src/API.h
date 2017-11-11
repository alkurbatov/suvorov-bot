#ifndef API_H
#define API_H

#include <sc2api/sc2_interfaces.h>

struct Order;

namespace API
{

struct Action
{
    explicit Action(sc2::ActionInterface* action_);

    void command(const Order& order_);

    void command(const Order& order_, const sc2::Point2D& point_);

    void attack(const sc2::Units& units_, const sc2::Point2D& point_);

private:
    sc2::ActionInterface* m_action;
};

struct Observer
{
    explicit Observer(const sc2::ObservationInterface* observer_);

    sc2::Units getUnits(const sc2::Filter& filter_,
        sc2::Unit::Alliance alliance_ = sc2::Unit::Alliance::Self) const;

    size_t countUnitType(sc2::UNIT_TYPEID type_) const;

    const sc2::GameInfo& gameInfo() const;

    sc2::Point3D startingLocation() const;

    int32_t getMinerals() const;

    int32_t getVespene() const;

    int32_t getAvailableFood() const;

private:
    const sc2::ObservationInterface* m_observer;
};

struct Query
{
    explicit Query(sc2::QueryInterface* query_);

    bool canBePlaced(const Order& order_, const sc2::Point2D& point_);

private:
    sc2::QueryInterface* m_query;
};

struct Interface
{
    Interface(sc2::ActionInterface* action_,
        const sc2::ObservationInterface* observer_,
        sc2::QueryInterface* query_);

    Action action() const;

    Observer observer() const;

    Query query() const;

private:
    sc2::ActionInterface* m_action;
    const sc2::ObservationInterface* m_observer;
    sc2::QueryInterface* m_query;
};

} // namespace API

extern std::unique_ptr<API::Interface> gAPI;

#endif
