#include <sc2api/sc2_unit.h>
#include <sc2api/sc2_common.h>
#include <sc2api/sc2_typeenums.h>
#include <sc2api/sc2_interfaces.h>

#include "Dispatcher.h"

namespace
{

sc2::UnitTypeID toUnitTypeID(sc2::UNIT_TYPEID id_)
{
    return static_cast<sc2::UnitTypeID>(id_);
}

} // namespace

Dispatcher::Dispatcher(): m_builder(Actions(), Observation(), Query()),
    m_overseer(Observation()) {
}

void Dispatcher::OnGameStart()
{
    BOOST_LOG_SEV(m_logger, info) << "New Game started!";

    m_startLocation = Observation()->GetStartLocation();

    auto& data = Observation()->GetUnitTypeData();

    // Initial build order
    m_orders.emplace(data[toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT)]);
    m_orders.emplace(data[toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_BARRACKS)]);
    m_orders.emplace(data[toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT)]);
    m_orders.emplace(data[toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_BARRACKS)]);
    m_orders.emplace(data[toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_BARRACKS)]);
    m_orders.emplace(data[toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_BARRACKS)]);
    m_orders.emplace(data[toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT)]);
    m_orders.emplace(data[toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT)]);
}

void Dispatcher::OnBuildingConstructionComplete(const sc2::Unit* building_)
{
    BOOST_LOG_SEV(m_logger, info) <<
        "Loop Step #" << Observation()->GetGameLoop() <<
        ": Building was created, tag: " << building_->tag;
}

void Dispatcher::OnStep()
{
    int32_t minerals = Observation()->GetMinerals();
    int32_t vespene = Observation()->GetVespene();

    while (!m_orders.empty()) {
        Order order = m_orders.front();

        if (!m_overseer.techRequirementMet(order.m_techRequirement))
            break;

        if (minerals < order.m_mineralCost || vespene < order.m_vespeneCost)
            break;

        // FIXME: check that we have enough supply to build a unit.

        if (!m_builder.execute(m_startLocation, order))
            break;

        minerals -= order.m_mineralCost;
        vespene -= order.m_vespeneCost;

        m_orders.pop();
    }

    // FIXME: skip this if we've planned additional supply already.
    // FIXME: skip this if we have 200 cap limit.
    // If we are not supply capped, don't build a supply depot.
    //if (m_overseer.hasSupply())
    //    return;

    //m_orders.emplace(Observation()->GetUnitTypeData()[toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT)]);
}

void Dispatcher::OnUnitCreated(const sc2::Unit* unit_)
{
    BOOST_LOG_SEV(m_logger, info) <<
        "Loop Step #" << Observation()->GetGameLoop() <<
        ": Unit was created, tag: " << unit_->tag;
}

void Dispatcher::OnUnitIdle(const sc2::Unit* unit_)
{
    switch (unit_->unit_type.ToType())
    {
        case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER:
        {
            // If we can add more SCVs do it.
            if (unit_->assigned_harvesters == 0 || unit_->assigned_harvesters < unit_->ideal_harvesters)
                m_orders.emplace(Observation()->GetUnitTypeData()[toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_SCV)], unit_);

            break;
        }

        case sc2::UNIT_TYPEID::TERRAN_SCV:
        {
            const sc2::Unit *mineral_target = m_overseer.findNearestMineralPatch(m_startLocation);
            if (!mineral_target)
                break;

            Actions()->UnitCommand(unit_, sc2::ABILITY_ID::SMART, mineral_target);
            break;
        }

        case sc2::UNIT_TYPEID::TERRAN_BARRACKS:
        {
            m_orders.emplace(Observation()->GetUnitTypeData()[toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_MARINE)], unit_);
            break;
        }

        case sc2::UNIT_TYPEID::TERRAN_MARINE:
        {
            if (m_overseer.countUnitType(sc2::UNIT_TYPEID::TERRAN_MARINE) < 13)
                break;

            Actions()->UnitCommand(unit_, sc2::ABILITY_ID::ATTACK_ATTACK, m_overseer.getEnemyStartingLocation());
            break;
        }

        default:
        {
            break;
        }
    }
}
