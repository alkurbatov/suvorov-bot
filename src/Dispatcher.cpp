#include <sc2api/sc2_unit.h>
#include <sc2api/sc2_common.h>
#include <sc2api/sc2_typeenums.h>
#include <sc2api/sc2_interfaces.h>

#include "Dispatcher.h"

Dispatcher::Dispatcher(): m_builder(Actions(), Observation()),
    m_overseer(Observation()) {
}

void Dispatcher::OnGameStart()
{
    BOOST_LOG_SEV(m_logger, info) << "New Game started!";

    m_startLocation = Observation()->GetStartLocation();
}

void Dispatcher::OnStep()
{
    TryBuildSupplyDepot();
    TryBuildBarracks();
}

void Dispatcher::OnUnitCreated(const sc2::Unit* unit)
{
    BOOST_LOG_SEV(m_logger, info) <<
        "Loop Step #" << Observation()->GetGameLoop() <<
        ": Unit was created, tag: " << unit->tag;
}

void Dispatcher::OnUnitIdle(const sc2::Unit* unit)
{
    switch (unit->unit_type.ToType())
    {
        case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER:
        {
            // unit->ideal_harvesters equal to zero
            // on game start but we need to start building SCVs anyways.
            if (unit->ideal_harvesters == 0 ||
                unit->assigned_harvesters < unit->ideal_harvesters)
            {
                Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_SCV);
            }

            break;
        }

        case sc2::UNIT_TYPEID::TERRAN_SCV:
        {
            const sc2::Unit *mineral_target = m_overseer.findNearestMineralPatch(m_startLocation);
            if (!mineral_target)
                break;

            Actions()->UnitCommand(unit, sc2::ABILITY_ID::SMART, mineral_target);
            break;
        }

        case sc2::UNIT_TYPEID::TERRAN_BARRACKS:
        {
            Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_MARINE);
            break;
        }

        case sc2::UNIT_TYPEID::TERRAN_MARINE:
        {
            if (m_overseer.countUnitType(sc2::UNIT_TYPEID::TERRAN_MARINE) < 13)
                break;

            Actions()->UnitCommand(unit, sc2::ABILITY_ID::ATTACK_ATTACK, m_overseer.getEnemyStartingLocation());
            break;
        }

        default:
        {
            break;
        }
    }
}

void Dispatcher::TryBuildSupplyDepot()
{
    // If we are not supply capped, don't build a supply depot.
    if (m_overseer.hasSupply())
        return;

    m_builder.buildStructure(m_startLocation, sc2::ABILITY_ID::BUILD_SUPPLYDEPOT);
}

void Dispatcher::TryBuildBarracks()
{
    if (m_overseer.countUnitType(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT) < 1) {
        return;
    }

    if (m_overseer.countUnitType(sc2::UNIT_TYPEID::TERRAN_BARRACKS) > 3) {
        return;
    }

    m_builder.buildStructure(m_startLocation, sc2::ABILITY_ID::BUILD_BARRACKS);
}
