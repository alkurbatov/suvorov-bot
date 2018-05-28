// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "World.h"

void World::OnBuildingConstructionComplete(const sc2::Unit& building_) {
    switch (building_.unit_type.ToType()) {
        case sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR:
        case sc2::UNIT_TYPEID::TERRAN_REFINERY:
        case sc2::UNIT_TYPEID::ZERG_EXTRACTOR:
            m_captured_geysers.insert(building_.tag);
            return;

        default:
            return;
    }
}

bool World::IsCapturedGeyser(sc2::Tag tag_) const {
    if (m_captured_geysers.empty())
        return false;

    return m_captured_geysers.end() != m_captured_geysers.find(tag_);
}

std::unique_ptr<World> gWorld;
