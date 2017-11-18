#include "API.h"
#include "ForceCommander.h"
#include "Pathfinder.h"

ForceCommander::ForceCommander(): m_attackStarted(false)
{}

void ForceCommander::onStep()
{
    // Clean up dead bodies.
    auto it = std::remove_if(m_units.begin(), m_units.end(),
        [](const sc2::Unit* unit_) {
            return !unit_->is_alive;
        }
    );

    m_units.erase(it, m_units.end());

    if (m_attackStarted) {
        // Send the attack order to units that have been produced recently.
        gAPI->action().attack(m_units, Pathfinder::getEnemyBaseLocation());
        return;
    }

    if (m_units.size() > 13)
    {
        m_attackStarted = true;
        gAPI->action().attack(m_units, Pathfinder::getEnemyBaseLocation());
    }
}

void ForceCommander::OnUnitCreated(const sc2::Unit* unit_)
{
    if (unit_->unit_type.ToType() != sc2::UNIT_TYPEID::TERRAN_MARINE)
        return;

    m_units.push_back(unit_);
}
