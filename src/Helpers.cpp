#include <cmath>

#include <sc2api/sc2_unit.h>

#include "Helpers.h"

isUnit::isUnit(sc2::UNIT_TYPEID type_): m_type(type_)
{}

bool isUnit::operator()(const sc2::Unit& unit_)
{
    // Don't report the units under construction.
    return unit_.unit_type == m_type && unit_.build_progress == 1.0f;
}

bool isFreeWorker::operator()(const sc2::Unit& unit_)
{
    if (unit_.unit_type != sc2::UNIT_TYPEID::TERRAN_SCV)
        return false;

    auto it = std::find_if(unit_.orders.begin(), unit_.orders.end(), isBuildingOrder());
    return it == unit_.orders.end();
}

bool isBuildingOrder::operator()(const sc2::UnitOrder& order_)
{
    switch(static_cast<sc2::ABILITY_ID>(order_.ability_id)) {
        case sc2::ABILITY_ID:: BUILD_COMMANDCENTER:
        case sc2::ABILITY_ID:: BUILD_SUPPLYDEPOT:
        case sc2::ABILITY_ID:: BUILD_REFINERY:
        case sc2::ABILITY_ID:: BUILD_BARRACKS:
        case sc2::ABILITY_ID:: BUILD_ENGINEERINGBAY:
        case sc2::ABILITY_ID:: BUILD_MISSILETURRET:
        case sc2::ABILITY_ID:: BUILD_BUNKER:
        case sc2::ABILITY_ID:: BUILD_SENSORTOWER:
        case sc2::ABILITY_ID:: BUILD_GHOSTACADEMY:
        case sc2::ABILITY_ID:: BUILD_FACTORY:
        case sc2::ABILITY_ID:: BUILD_STARPORT:
        case sc2::ABILITY_ID:: BUILD_ARMORY:
        case sc2::ABILITY_ID:: BUILD_FUSIONCORE:
            return true;

        default:
            return false;
    }
}