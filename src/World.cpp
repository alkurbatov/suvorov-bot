// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "Helpers.h"
#include "World.h"

void World::OnUnitCreated(const sc2::Unit& unit_) {
    switch (unit_.unit_type.ToType()) {
        case sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR:
        case sc2::UNIT_TYPEID::TERRAN_REFINERY:
        case sc2::UNIT_TYPEID::ZERG_EXTRACTOR: {
            auto it = m_captured_geysers.find(Object(unit_));
            if (m_captured_geysers.end() != it)
                m_captured_geysers.erase(it);  // might be claimed geyser

            m_captured_geysers.emplace(unit_);
            return;
        }

        default:
            return;
    }
}

bool World::IsOccupied(const sc2::Unit& unit_) const {
    if (m_captured_geysers.empty())
        return false;

    return m_captured_geysers.end() != m_captured_geysers.find(Object(unit_));
}

bool World::IsTargetOccupied(const sc2::UnitOrder& order_) const {
    if (m_captured_geysers.empty())
        return false;

    return m_captured_geysers.end() != m_captured_geysers.find(Object(order_));
}

void World::ClaimObject(const sc2::Unit& unit_) {
    if (IsGeyser()(unit_))
        m_captured_geysers.emplace(unit_);
}

std::unique_ptr<World> gWorld;
