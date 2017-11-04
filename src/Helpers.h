#ifndef HELPERS_H
#define HELPERS_H

#include <sc2api/sc2_common.h>

namespace sc2
{
class Unit;
} // namespace sc2

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
