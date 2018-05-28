// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_gametypes.h>
#include <sc2api/sc2_unit.h>

#include <memory>
#include <set>

struct World {
    void OnBuildingConstructionComplete(const sc2::Unit& building_);

    bool IsCapturedGeyser(sc2::Tag tag_) const;

 private:
    std::set<sc2::Tag> m_captured_geysers;
};

extern std::unique_ptr<World> gWorld;
