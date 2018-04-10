// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "ForceCommander.h"
#include "Pathfinder.h"

void ForceCommander::OnStep() {
    // Clean up dead bodies.
    auto it = std::remove_if(m_units.begin(), m_units.end(),
        [](const sc2::Unit* unit_) {
            return !unit_->is_alive;
        });

    m_units.erase(it, m_units.end());

    if (m_units.size() > 13) {
        gAPI->action().Attack(m_units, Pathfinder::GetEnemyBaseLocation());
        m_units.clear();
    }
}

void ForceCommander::OnUnitCreated(const sc2::Unit* unit_) {
    if (unit_->unit_type.ToType() != sc2::UNIT_TYPEID::TERRAN_MARINE)
        return;

    m_units.push_back(unit_);
}
