// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Order.h"

#include <sc2api/sc2_common.h>
#include <sc2api/sc2_unit.h>

struct IsUnit {
    explicit IsUnit(sc2::UNIT_TYPEID type_, bool with_not_finished = false);

    bool operator()(const sc2::Unit& unit_) const;

 private:
    sc2::UNIT_TYPEID m_type;
    float m_build_progress;
};

struct IsCombatUnit {
    bool operator()(const sc2::Unit& unit_) const;
};

// FIXME(alkurbatov): Check that the provided unit is not depleted mineral patch
struct IsMineralPatch {
    bool operator()(const sc2::Unit& unit_) const;
};

// Check that the provided unit is not depleted geyser
struct IsGeyser {
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

struct IsWorker {
    bool operator()(const sc2::Unit& unit_) const;
};

struct IsGasWorker {
    bool operator()(const sc2::Unit& unit_) const;
};

struct IsTownHall {
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

struct IsTrainingWorkers {
    bool operator()(const sc2::UnitOrder& order_) const;
};
