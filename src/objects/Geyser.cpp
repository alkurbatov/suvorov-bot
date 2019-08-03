// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#include "Geyser.h"

Geyser::Geyser(const sc2::Unit& unit_): GameObject(unit_), m_pos(unit_.pos) {
}

Geyser::Geyser(const sc2::UnitOrder& order_): GameObject(order_.target_unit_tag),
    m_pos(order_.target_pos) {
}

bool Geyser::operator==(const Geyser& geyser_) const {
    return this->Tag() == geyser_.Tag() ||
        (this->m_pos.x == geyser_.m_pos.x && this->m_pos.y == geyser_.m_pos.y);
}
