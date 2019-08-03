// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#pragma once

#include <string>

#include <sc2api/sc2_gametypes.h>
#include <sc2api/sc2_typeenums.h>

// The collection of type conversion utilities
namespace convert {

inline sc2::UnitTypeID ToUnitTypeID(sc2::UNIT_TYPEID id_) {
    return static_cast<sc2::UnitTypeID>(id_);
}

inline sc2::UpgradeID ToUpgradeID(sc2::UPGRADE_ID id_) {
    return static_cast<sc2::UpgradeID>(id_);
}

inline sc2::AbilityID ToAbilityID(sc2::ABILITY_ID id_) {
    return static_cast<sc2::AbilityID>(id_);
}

sc2::Difficulty StringToDifficulty(const std::string& name_);

sc2::Race StringToRace(const std::string& name_);

}  // namespace convert
