// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../Historican.h"
#include "ForceCommander.h"
#include "core/API.h"
#include "core/Helpers.h"

#include <sc2api/sc2_map_info.h>

#include <algorithm>

ForceCommander::ForceCommander(): m_attack_limit(16) {
}

void ForceCommander::OnStep(Builder*) {
    // Clean up dead bodies.
    auto it = std::remove_if(m_units.begin(), m_units.end(),
        [](const sc2::Unit* unit_) {
            return !unit_->is_alive;
        });

    m_units.erase(it, m_units.end());

    if (m_units.size() < m_attack_limit)
        return;

    auto targets = gAPI->observer().GameInfo().enemy_start_locations;
    gAPI->action().Attack(m_units, targets.front());

    m_units.clear();
    m_attack_limit = std::min<float>(m_attack_limit * 1.5f, 170.0f);
}

void ForceCommander::OnUnitCreated(const sc2::Unit* unit_) {
    if (!IsCombatUnit()(*unit_))
        return;

    gHistory.info() << sc2::UnitTypeToName(unit_->unit_type) <<
        " added to attack group" << std::endl;

    m_units.push_back(unit_);
}
