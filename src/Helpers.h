#pragma once

#include <sc2api/sc2_common.h>
#include <sc2api/sc2_unit.h>

struct IsUnit {
    explicit IsUnit(sc2::UNIT_TYPEID type_);

    bool operator()(const sc2::Unit& unit_);

private:
    sc2::UNIT_TYPEID m_type;
};

// Check that the provided unit is not depleted mineral patch
struct IsMineralPatch {
    bool operator()(const sc2::Unit& unit_);
};

// Check that the provided unit is not depleted geyser
struct IsGeiser {
    bool operator()(const sc2::Unit& unit_);
};

struct IsFreeWorker {
    bool operator()(const sc2::Unit& unit_);
};

struct IsBuildingOrder {
    bool operator()(const sc2::UnitOrder& order_);
};
