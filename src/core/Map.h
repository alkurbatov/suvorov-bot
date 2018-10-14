// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_unit.h>

#include <list>
#include <vector>

struct Expansion {
    explicit Expansion(const sc2::Point3D& town_hall_location_);

    sc2::Point3D town_hall_location;
};

typedef std::vector<Expansion> Expansions;

// NOTE (alkurbatov): Slightly optimised version of the builtin function.
Expansions CalculateExpansionLocations();
