#include <sc2api/sc2_interfaces.h>

#include "Builder.h"

Builder::Builder(sc2::ActionInterface* action_,
    const sc2::ObservationInterface* observation_):
    m_action(action_), m_observation(observation_)
{}

void Builder::buildStructure(const sc2::Point2D& starting_point, sc2::ABILITY_ID ability_type)
{
    const sc2::Unit* unit_to_build = nullptr;

    sc2::Units units = m_observation->GetUnits(sc2::Unit::Alliance::Self);
    for (const auto& unit : units)
    {
        if (unit->unit_type != sc2::UNIT_TYPEID::TERRAN_SCV)
            continue;

        for (const auto& order : unit->orders)
        {
            // If a unit already is building a structure of this type, do nothing.
            if (order.ability_id == ability_type)
                return;
        }

        if (!unit_to_build)
            unit_to_build = unit;
    }

    if (!unit_to_build)
        return;

    float rx = sc2::GetRandomScalar();
    float ry = sc2::GetRandomScalar();

    m_action->UnitCommand(unit_to_build, ability_type,
        sc2::Point2D(starting_point.x + rx * 15.0f, starting_point.y + ry * 15.0f));
}
