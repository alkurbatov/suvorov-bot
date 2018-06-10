// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "Helpers.h"
#include "World.h"

World::World(sc2::Race current_race_): m_current_race(current_race_),
    m_current_worker_type(sc2::UNIT_TYPEID::INVALID) {
    switch (m_current_race) {
        case sc2::Race::Protoss:
            m_current_worker_type = sc2::UNIT_TYPEID::PROTOSS_PROBE;
            return;

        case sc2::Race::Terran:
            m_current_worker_type = sc2::UNIT_TYPEID::TERRAN_SCV;
            return;

        case sc2::Race::Zerg:
            m_current_worker_type = sc2::UNIT_TYPEID::ZERG_DRONE;
            return;

        default:
            return;
    }
}

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

void World::OnUnitDestroyed(const sc2::Unit* unit_) {
    switch (unit_->unit_type.ToType()) {
        case sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR:
        case sc2::UNIT_TYPEID::TERRAN_REFINERY:
        case sc2::UNIT_TYPEID::ZERG_EXTRACTOR: {
            auto it = m_captured_geysers.find(Object(*unit_));

            if (m_captured_geysers.end() != it)
                m_captured_geysers.erase(it);
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

sc2::Race World::GetCurrentRace() const {
    return m_current_race;
}

sc2::UNIT_TYPEID World::GetCurrentWorkerType() const {
    return m_current_worker_type;
}

std::unique_ptr<World> gWorld;
