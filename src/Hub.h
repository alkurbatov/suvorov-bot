// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "objects/Geyser.h"
#include "objects/Worker.h"

#include <sc2api/sc2_gametypes.h>
#include <sc2api/sc2_unit.h>

#include <list>
#include <unordered_set>

template <typename T>
struct Cache {
    bool Empty() const;

    void Add(const T& obj_);

    T& Back();

    bool Swap(const T& obj_, Cache<T>& dst_);

    bool Remove(const T& obj_);

    T* GetClosestTo(const sc2::Point2D& location_);

 private:
    std::list<T> m_objects;
};

template <typename T>
bool Cache<T>::Empty() const {
    return m_objects.empty();
}

template <typename T>
void Cache<T>::Add(const T& obj_) {
    m_objects.push_back(obj_);
}

template <typename T>
T& Cache<T>::Back() {
    return m_objects.back();
}

template <typename T>
bool Cache<T>::Swap(const T& obj_, Cache<T>& dst_) {
    auto it = std::find(m_objects.begin(), m_objects.end(), obj_);

    if (m_objects.end() == it)
        return false;

    dst_.Add(*it);
    m_objects.erase(it);
    return true;
}

template <typename T>
bool Cache<T>::Remove(const T& obj_) {
    auto it = std::find(m_objects.begin(), m_objects.end(), obj_);
    if (m_objects.end() == it)
        return false;

    m_objects.erase(it);
    return true;
}

template <typename T>
T* Cache<T>::GetClosestTo(const sc2::Point2D& location_) {
    auto closest_worker = m_objects.end();
    float distance = std::numeric_limits<float>::max();

    for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
        float d = sc2::DistanceSquared2D(it->GetPos(), location_);

        if (d >= distance)
            continue;

        distance = d;
        closest_worker = it;
    }

    if (closest_worker == m_objects.end())
        return nullptr;

    return &(*closest_worker);
}

struct Hub {
    explicit Hub(sc2::Race current_race_);

    void OnStep();

    void OnUnitCreated(const sc2::Unit& unit_);

    void OnUnitDestroyed(const sc2::Unit& unit_);

    void OnUnitIdle(const sc2::Unit& unit_);

    bool IsOccupied(const sc2::Unit& unit_) const;

    bool IsTargetOccupied(const sc2::UnitOrder& order_) const;

    void ClaimObject(const sc2::Unit& unit_);

    sc2::Race GetCurrentRace() const;

    Worker* GetClosestFreeWorker(const sc2::Point2D& location_);

    sc2::UNIT_TYPEID GetCurrentWorkerType() const;

    bool AssignRefineryConstruction(Order* order_, const sc2::Unit* geyser_);

    bool AssignBuildTask(Order* order_, const sc2::Point2D& point_);

    void AssignVespeneHarvester(const sc2::Unit& refinery_);

 private:
    sc2::Race m_current_race;
    sc2::UNIT_TYPEID m_current_worker_type;

    std::unordered_set<Geyser, GameObjectHasher> m_captured_geysers;

    Cache<Worker> m_busy_workers;
    Cache<Worker> m_free_workers;
};

extern std::unique_ptr<Hub> gHub;
