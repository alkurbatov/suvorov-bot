#include "API.h"
#include "ForceCommander.h"
#include "Pathfinder.h"

ForceCommander::ForceCommander(): m_attack_started(false) {
}

void ForceCommander::OnStep() {
    // Clean up dead bodies.
    auto it = std::remove_if(m_units.begin(), m_units.end(),
        [](const sc2::Unit* unit_) {
            return !unit_->is_alive;
        });

    m_units.erase(it, m_units.end());

    if (m_attack_started) {
        // Send the attack order to units that have been produced recently.
        gAPI->action().Attack(m_units, Pathfinder::GetEnemyBaseLocation());
        return;
    }

    if (m_units.size() > 13) {
        m_attack_started = true;
        gAPI->action().Attack(m_units, Pathfinder::GetEnemyBaseLocation());
    }
}

void ForceCommander::OnUnitCreated(const sc2::Unit* unit_) {
    if (unit_->unit_type.ToType() != sc2::UNIT_TYPEID::TERRAN_MARINE)
        return;

    m_units.push_back(unit_);
}
