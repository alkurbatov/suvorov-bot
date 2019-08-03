// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#include "Converter.h"

namespace convert {

sc2::Difficulty StringToDifficulty(const std::string& name_) {
    if (name_ == "VeryEasy")
        return sc2::Difficulty::VeryEasy;

    if (name_ == "Easy")
        return sc2::Difficulty::Easy;

    if (name_ == "Medium")
        return sc2::Difficulty::Medium;

    if (name_ == "MediumHard")
        return sc2::Difficulty::MediumHard;

    if (name_ == "Hard")
        return sc2::Difficulty::Hard;

    if (name_ == "HardVeryHard")
        return sc2::Difficulty::HardVeryHard;

    if (name_ == "VeryHard")
        return sc2::Difficulty::VeryHard;

    if (name_ == "CheatVision")
        return sc2::Difficulty::CheatVision;

    if (name_ == "CheatMoney")
        return sc2::Difficulty::CheatMoney;

    if (name_ == "CheatInsane")
        return sc2::Difficulty::CheatInsane;

    return sc2::Difficulty::Easy;
}

sc2::Race StringToRace(const std::string& name_) {
    std::string race(name_);
    std::transform(race.begin(), race.end(), race.begin(), ::tolower);

    if (race == "terran")
        return sc2::Race::Terran;

    if (race == "protoss")
        return sc2::Race::Protoss;

    if (race == "zerg")
        return sc2::Race::Zerg;

    if (race == "random")
        return sc2::Race::Random;

    return sc2::Race::Random;
}

}  // namespace convert
