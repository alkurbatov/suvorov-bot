// The MIT License (MIT)
//
// Copyright (c) 2017-2022 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_typeenums.h>

#include <exception>
#include <string>

struct InvalidBuildCommand : std::runtime_error {
    explicit InvalidBuildCommand(sc2::ABILITY_ID id_): std::runtime_error(
        "Invalid build command " + std::to_string(static_cast<int>(id_))) {
    }
};
