#ifndef CONVERTER_H
#define CONVERTER_H

#include <sc2api/sc2_typeenums.h>

// The Collection of type conversion utilities
struct Convert
{

static sc2::UnitTypeID toUnitTypeID(sc2::UNIT_TYPEID id_)
{
    return static_cast<sc2::UnitTypeID>(id_);
}

static sc2::ABILITY_ID toAbilityID(sc2::AbilityID id_)
{
    return static_cast<sc2::ABILITY_ID>(id_);
}

};

#endif
