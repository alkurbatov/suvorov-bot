#pragma once

#include "Historican.h"
#include "Order.h"

#include <sc2api/sc2_interfaces.h>

#include <memory>

namespace API {

struct Action {
    explicit Action(sc2::ActionInterface* action_);

    void Command(const Order& order_);

    void Command(const Order& order_, const sc2::Unit* unit_);

    void Command(const Order& order_, const sc2::Point2D& point_);

    void Attack(const sc2::Units& units_, const sc2::Point2D& point_);

private:
    sc2::ActionInterface* m_action;
};

struct Observer {
    explicit Observer(const sc2::ObservationInterface* observer_);

    sc2::Units GetUnits(const sc2::Filter& filter_,
        sc2::Unit::Alliance alliance_ = sc2::Unit::Alliance::Self) const;

    const sc2::Unit* GetClosestUnit(const sc2::Point2D& point_,
        const sc2::Filter& filter_, sc2::Unit::Alliance alliance_) const;

    size_t CountUnitType(sc2::UNIT_TYPEID type_) const;

    const sc2::GameInfo& GameInfo() const;

    sc2::Point3D StartingLocation() const;

    int32_t GetMinerals() const;

    int32_t GetVespene() const;

    float GetAvailableFood() const;

private:
    const sc2::ObservationInterface* m_observer;
};

struct Query {
    explicit Query(sc2::QueryInterface* query_);

    bool CanBePlaced(const Order& order_, const sc2::Point2D& point_);

private:
    sc2::QueryInterface* m_query;
};

struct Interface {
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

}  // namespace API

extern std::unique_ptr<API::Interface> gAPI;
