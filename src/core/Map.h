// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

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

    void AddResource(const sc2::Unit* resource_);

    const sc2::Unit* getClosestMineralTo(const sc2::Point2D& point_) const;

    sc2::Point3D town_hall_location;
    Owner owner;
    sc2::Tag town_hall_tag;  // valid for Owner::SELF or ENEMY
    // NOTE (impulsecloud): check for dead builder, send new
    std::vector<sc2::Tag> minerals;
    std::vector<sc2::Tag> geysers;
};

typedef std::vector<Expansion> Expansions;

// NOTE (alkurbatov): Slightly optimised version of the builtin function.
Expansions CalculateExpansionLocations();
