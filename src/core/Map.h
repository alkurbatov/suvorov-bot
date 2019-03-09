// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_unit.h>

#include <list>
#include <vector>

enum Owner {
    NEUTRAL = 0,
    CONTESTED = 1,
    ENEMY = 2,
    SELF = 3,
};

struct Expansion {
    explicit Expansion(const sc2::Point3D& town_hall_location_);

    sc2::Point3D town_hall_location;
    Owner owner;
};

typedef std::vector<Expansion> Expansions;

// NOTE (alkurbatov): Slightly optimised version of the builtin function.
Expansions CalculateExpansionLocations();
