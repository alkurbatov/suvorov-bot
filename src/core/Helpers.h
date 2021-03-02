// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

#pragma once

#include "Order.h"

#include <sc2api/sc2_common.h>
#include <sc2api/sc2_unit.h>

#include <set>

const float BUILD_FINISHED = 1.0f;

struct IsUnit {
    explicit IsUnit(sc2::UNIT_TYPEID type_);

    bool operator()(const sc2::Unit& unit_) const;

 private:
    sc2::UNIT_TYPEID m_type;
};

struct OneOfUnits {
    explicit OneOfUnits(const std::set<sc2::UNIT_TYPEID>& types_);

    bool operator()(const sc2::Unit& unit_) const;

 private:
    const std::set<sc2::UNIT_TYPEID> m_types;
};

struct IsCombatUnit {
    bool operator()(const sc2::Unit& unit_) const;
};

struct IsFoggyResource {
    bool operator()(const sc2::Unit& unit_) const;
};

// Check that the provided unit is not occupied and not depleted geyser
struct IsFreeGeyser {
    bool operator()(const sc2::Unit& unit_) const;
};

struct IsRefinery {
    bool operator()(const sc2::Unit& unit_) const;
};

struct IsIdleUnit {
    explicit IsIdleUnit(sc2::UNIT_TYPEID type_);

    bool operator()(const sc2::Unit& unit_) const;

 private:
    sc2::UNIT_TYPEID m_type;
};

struct IsGasWorker {
    bool operator()(const sc2::Unit& unit_) const;
};

struct IsIdleTownHall {
    bool operator()(const sc2::Unit& unit_) const;
};

struct IsCommandCenter {
    bool operator()(const sc2::Unit& unit_) const;
};

struct IsOrdered {
    explicit IsOrdered(sc2::UNIT_TYPEID type_);

    bool operator()(const Order& order_) const;

 private:
    sc2::UNIT_TYPEID m_type;
};
