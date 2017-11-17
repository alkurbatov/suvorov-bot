#ifndef HELPERS_H
#define HELPERS_H

#include <sc2api/sc2_common.h>
#include <sc2api/sc2_unit.h>

struct isUnit
{
    explicit isUnit(sc2::UNIT_TYPEID type_);

    bool operator()(const sc2::Unit& unit_);

private:
    sc2::UNIT_TYPEID m_type;
};

struct isFreeWorker
{
    bool operator()(const sc2::Unit& unit_);
};

struct isBuildingOrder
{
    bool operator()(const sc2::UnitOrder& order_);
};

#endif
