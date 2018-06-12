// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "Helpers.h"
#include "World.h"

Object::Object(const sc2::Unit& unit_): tag(unit_.tag), pos(unit_.pos) {
}

Object::Object(const sc2::UnitOrder& order_): tag(order_.target_unit_tag),
    pos(order_.target_pos) {
}

bool Object::operator==(const Object& obj_) const {
    return this->tag == obj_.tag ||
        (this->pos.x == obj_.pos.x && this->pos.y == obj_.pos.y);
}

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

void World::OnStep() {
    m_free_workers = gAPI->observer().GetUnits(IsFreeWorker());
}

void World::OnUnitCreated(const sc2::Unit& unit_) {
    switch (unit_.unit_type.ToType()) {
        case sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR:
        case sc2::UNIT_TYPEID::TERRAN_REFINERY:
        case sc2::UNIT_TYPEID::ZERG_EXTRACTOR: {
            Object obj(unit_);

            auto it = find(m_captured_geysers.begin(), m_captured_geysers.end(), obj);
            if (m_captured_geysers.end() != it)
                m_captured_geysers.erase(it);  // might be claimed geyser

            m_captured_geysers.insert(obj);
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

const sc2::Unit* World::GetFreeWorker() {
    if (m_free_workers.empty())
        return nullptr;

    const sc2::Unit* worker =  m_free_workers.back();
    m_free_workers.pop_back();
    return worker;
}

const sc2::Unit* World::GetClosestFreeWorker(const sc2::Point2D& location_) {
    auto index = m_free_workers.end();
    float distance = std::numeric_limits<float>::max();

    for (auto it = m_free_workers.begin(); it != m_free_workers.end(); ++it) {
        float d = sc2::DistanceSquared2D((*it)->pos, location_);

        if (d >= distance)
            continue;

        distance = d;
        index = it;
    }

    if (index == m_free_workers.end())
        return nullptr;

    const sc2::Unit* worker = *index;
    m_free_workers.erase(index);
    return worker;
}

sc2::UNIT_TYPEID World::GetCurrentWorkerType() const {
    return m_current_worker_type;
}

std::unique_ptr<World> gWorld;
