// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_unit.h>

#include <list>
#include <vector>

enum Owner {
    NEUTRAL = 0,  // No Townhall at expansion, no workers enroute
    CONTESTED = 1,  // No Townhall, Enemy may be present, Self Worker enroute
    ENEMY = 2,  // Enemy TownHall at expansion
    SELF = 3,  // Self TownHall at expansion, possibly under construction
};

struct Expansion {
    explicit Expansion(const sc2::Point3D& town_hall_location_);

    void SetOwner(const sc2::Unit& unit_, Owner owner_);

    void RemoveOwner();

    sc2::Point3D town_hall_location;
    Owner owner;
    sc2::Tag town_hall_tag;  // valid for Owner::SELF or ENEMY
    sc2::Tag worker_tag;  // valid for Owner::CONTESTED or SELF
    // NOTE (impulsecloud): check for dead builder, send new
};

typedef std::vector<Expansion> Expansions;

// NOTE (alkurbatov): Slightly optimised version of the builtin function.
Expansions CalculateExpansionLocations();
