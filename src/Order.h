#pragma once

#include <sc2api/sc2_data.h>
#include <sc2api/sc2_unit.h>

struct Order
{
    explicit Order(const sc2::UnitTypeData& data_, const sc2::Unit* assignee_ = nullptr):
        m_data(data_), m_assignee(assignee_)
    {}

    sc2::UnitTypeData m_data; 
    const sc2::Unit* m_assignee;
};
