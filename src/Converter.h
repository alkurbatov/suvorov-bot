// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_typeenums.h>

// The collection of type conversion utilities
namespace convert {

inline sc2::UnitTypeID ToUnitTypeID(sc2::UNIT_TYPEID id_) {
    return static_cast<sc2::UnitTypeID>(id_);
}

inline sc2::ABILITY_ID ToAbilityID(sc2::AbilityID id_) {
    return static_cast<sc2::ABILITY_ID>(id_);
}

}  // namespace convert
