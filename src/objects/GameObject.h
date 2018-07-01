// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_gametypes.h>
#include <sc2api/sc2_unit.h>

struct GameObject {
    explicit GameObject(sc2::Tag tag_);

    explicit GameObject(const sc2::Unit& unit_);

    bool operator==(const GameObject& obj_) const;

    bool operator==(const sc2::Unit& unit_) const;

    sc2::Tag tag() const;

    sc2::Point3D GetPos() const;

    sc2::Unit ToUnit() const;

 private:
    sc2::Tag m_tag;
};

struct GameObjectHasher {
    size_t operator()(const GameObject& obj_) const {
        return std::hash<sc2::Tag>()(obj_.tag());
    }
};
