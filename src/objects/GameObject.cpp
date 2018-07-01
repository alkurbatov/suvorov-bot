// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "GameObject.h"

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

sc2::Tag GameObject::tag() const {
    return m_tag;
}

sc2::Point3D GameObject::GetPos() const {
     return gAPI->observer().GetUnit(m_tag)->pos;
 }

sc2::Unit GameObject::ToUnit() const {
    sc2::Unit unit;
    unit.tag = m_tag;

    return unit;
}
