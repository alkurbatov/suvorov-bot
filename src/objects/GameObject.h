// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_gametypes.h>
#include <sc2api/sc2_unit.h>

struct GameObject {
    explicit GameObject(sc2::Tag tag_);

    explicit GameObject(const sc2::Unit& unit_);

    virtual ~GameObject() = default;

    bool operator==(const GameObject& obj_) const;

    bool operator==(const sc2::Unit& unit_) const;

    sc2::Tag Tag() const;

    sc2::Point3D GetPos() const;

    sc2::Unit ToUnit() const;

    static sc2::Unit ToUnit(sc2::Tag tag_);

 private:
    sc2::Tag m_tag;
};
