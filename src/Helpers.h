#pragma once

#include <sc2api/sc2_common.h>
#include <sc2api/sc2_unit.h>

struct IsUnit {
    explicit IsUnit(sc2::UNIT_TYPEID type_);

    bool operator()(const sc2::Unit& unit_);

private:
    sc2::UNIT_TYPEID m_type;
};

struct IsFreeWorker {
    bool operator()(const sc2::Unit& unit_);
};

struct IsBuildingOrder {
    bool operator()(const sc2::UnitOrder& order_);
};
