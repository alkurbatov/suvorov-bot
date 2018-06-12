// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_gametypes.h>
#include <sc2api/sc2_unit.h>

#include <unordered_set>

struct Object {
    explicit Object(const sc2::Unit& unit_);

    explicit Object(const sc2::UnitOrder& order_);

    bool operator==(const Object& obj_) const;

    sc2::Tag tag;
    sc2::Point2D pos;
};

struct ObjectHasher {
    size_t operator()(const Object& obj_) const {
        return std::hash<sc2::Tag>()(obj_.tag);
    }
};

struct World {
    explicit World(sc2::Race current_race_);

    void OnStep();

    void OnUnitCreated(const sc2::Unit& unit_);

    void OnUnitDestroyed(const sc2::Unit* unit_);

    bool IsOccupied(const sc2::Unit& unit_) const;

    bool IsTargetOccupied(const sc2::UnitOrder& order_) const;

    void ClaimObject(const sc2::Unit& unit_);

    sc2::Race GetCurrentRace() const;

    const sc2::Unit* GetFreeWorker();

    const sc2::Unit* GetClosestFreeWorker(const sc2::Point2D& location_);

    sc2::UNIT_TYPEID GetCurrentWorkerType() const;

 private:
    sc2::Race m_current_race;
    sc2::UNIT_TYPEID m_current_worker_type;

    sc2::Units m_free_workers;
    std::unordered_set<Object, ObjectHasher> m_captured_geysers;
};

extern std::unique_ptr<World> gWorld;
