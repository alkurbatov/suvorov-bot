// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#include "GameObject.h"
#include "core/API.h"

GameObject::GameObject(sc2::Tag tag_): m_tag(tag_) {
}

GameObject::GameObject(const sc2::Unit& unit_): m_tag(unit_.tag) {
}

bool GameObject::operator==(const GameObject& obj_) const {
    return this->m_tag == obj_.m_tag;
}

bool GameObject::operator==(const sc2::Unit& unit_) const {
    return this->m_tag == unit_.tag;
}

sc2::Tag GameObject::Tag() const {
    return m_tag;
}

sc2::Point3D GameObject::GetPos() const {
    return gAPI->observer().GetUnit(m_tag)->pos;
}

sc2::Unit GameObject::ToUnit() const {
    return ToUnit(m_tag);
}

sc2::Unit GameObject::ToUnit(sc2::Tag tag_) {
    sc2::Unit unit;
    unit.tag = tag_;

    return unit;
}
