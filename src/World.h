// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_gametypes.h>
#include <sc2api/sc2_unit.h>

#include <memory>
#include <unordered_set>

struct Object {
    explicit Object(const sc2::Unit& unit_): tag(unit_.tag), pos(unit_.pos) {
    }

    sc2::Tag tag;
    sc2::Point3D pos;
};

struct ObjectHasher {
    size_t operator()(const Object& obj_) const {
        return std::hash<sc2::Tag>()(obj_.tag);
    }
};

struct ObjectComparator {
    bool operator()(const Object& obj1_, const Object& obj2_) const {
        return obj1_.pos.x == obj2_.pos.x && obj1_.pos.y == obj2_.pos.y;
    }
};

struct World {
    void OnUnitCreated(const sc2::Unit& unit_);

    bool IsOccupied(const sc2::Unit& unit_) const;

    void CaptureObject(const sc2::Unit& unit_);

 private:
    std::unordered_set<Object, ObjectHasher, ObjectComparator> m_captured_geysers;
};

extern std::unique_ptr<World> gWorld;
