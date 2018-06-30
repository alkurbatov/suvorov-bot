// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_gametypes.h>
#include <sc2api/sc2_unit.h>

struct GameObject {
    explicit GameObject(sc2::Tag tag_): m_tag(tag_) {
    }

    explicit GameObject(const sc2::Unit& unit_): m_tag(unit_.tag) {
    }

    bool operator==(const GameObject& obj_) const {
        return this->m_tag == obj_.m_tag;
    }

    bool operator==(const sc2::Unit& unit_) const {
        return this->m_tag == unit_.tag;
    }

    sc2::Tag tag() const {
        return m_tag;
    }

    sc2::Unit ToUnit() const {
        sc2::Unit unit;
        unit.tag = m_tag;

        return unit;
    }

 private:
    sc2::Tag m_tag;
};

struct GameObjectHasher {
    size_t operator()(const GameObject& obj_) const {
        return std::hash<sc2::Tag>()(obj_.tag());
    }
};
