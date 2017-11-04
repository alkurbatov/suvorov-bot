#ifndef ORDER_H
#define ORDER_H

#include <sc2api/sc2_data.h>
#include <sc2api/sc2_unit.h>

struct Order
{
    explicit Order(const sc2::UnitTypeData& data_): m_id(data_.ability_id),
        m_mineralCost(data_.mineral_cost), m_vespeneCost(data_.vespene_cost),
        m_techRequirement(data_.tech_requirement),
        m_assignee(nullptr)
    {}

    Order(const sc2::UnitTypeData& data_, const sc2::Unit* assignee_): m_id(data_.ability_id),
        m_mineralCost(data_.mineral_cost), m_vespeneCost(data_.vespene_cost),
        m_techRequirement(data_.tech_requirement),
        m_assignee(assignee_)
    {}

    sc2::ABILITY_ID m_id;

    int32_t m_mineralCost;
    int32_t m_vespeneCost;
    sc2::UnitTypeID m_techRequirement;

    const sc2::Unit* m_assignee;
};

#endif
