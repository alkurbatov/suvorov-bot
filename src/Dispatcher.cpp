#include <sc2api/sc2_unit.h>
#include <sc2api/sc2_common.h>
#include <sc2api/sc2_typeenums.h>

#include "API.h"
#include "Converter.h"
#include "Pathfinder.h"
#include "Dispatcher.h"

Dispatcher::Dispatcher()
{
    gAPI.reset(new API::Interface(Actions(), Observation(), Query()));
}

void Dispatcher::OnGameStart()
{
    BOOST_LOG_SEV(m_logger, info) << "New Game started!";

    auto& data = Observation()->GetUnitTypeData();

    // Initial build order
    m_constructionOrders.emplace(data[Convert::toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT)]);
    m_constructionOrders.emplace(data[Convert::toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_BARRACKS)]);
    m_constructionOrders.emplace(data[Convert::toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT)]);
    m_constructionOrders.emplace(data[Convert::toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_BARRACKS)]);
    m_constructionOrders.emplace(data[Convert::toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_BARRACKS)]);
    m_constructionOrders.emplace(data[Convert::toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_BARRACKS)]);
    m_constructionOrders.emplace(data[Convert::toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT)]);
    m_constructionOrders.emplace(data[Convert::toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT)]);
}

void Dispatcher::OnBuildingConstructionComplete(const sc2::Unit* building_)
{
    BOOST_LOG_SEV(m_logger, info) <<
        "Loop Step #" << Observation()->GetGameLoop() <<
        ": Building was created, tag: " << building_->tag;
}

void Dispatcher::OnStep()
{
    m_builder.onStep();
    m_forceCommander.onStep();

    while (!m_constructionOrders.empty()) {
        Order order = m_constructionOrders.front();

        if (!m_builder.buildStructure(order))
            break;

        m_constructionOrders.pop();
    }

    auto it = m_trainingOrders.begin();
    while (it != m_trainingOrders.end()) {
        if (!m_builder.trainUnit(*it)) {
            ++it;
            continue;
        }

        it = m_trainingOrders.erase(it);
    }

    // FIXME: skip this if we've planned additional supply already.
    // FIXME: skip this if we have 200 cap limit.
    // If we are not supply capped, don't build a supply depot.
    //size_t prediction = gAPI->observer().countUnitType(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT) * 2 + 2;
    //if (Observation()->GetFoodUsed() <= Observation()->GetFoodCap() - static_cast<int32_t>(prediction))
    //    return;

    //m_constructionOrders.emplace(Observation()->GetUnitTypeData()[toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT)]);
}

void Dispatcher::OnUnitCreated(const sc2::Unit* unit_)
{
    BOOST_LOG_SEV(m_logger, info) <<
        "Loop Step #" << Observation()->GetGameLoop() <<
        ": Unit was created, tag: " << unit_->tag;

    m_forceCommander.OnUnitCreated(unit_);
}

void Dispatcher::OnUnitIdle(const sc2::Unit* unit_)
{
    switch (unit_->unit_type.ToType())
    {
        case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER:
        {
            // If we can add more SCVs do it.
            if (unit_->assigned_harvesters == 0 || unit_->assigned_harvesters < unit_->ideal_harvesters)
                m_trainingOrders.emplace_back(Observation()->GetUnitTypeData()[
                    Convert::toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_SCV)], unit_);

            break;
        }

        case sc2::UNIT_TYPEID::TERRAN_SCV:
        {
            const sc2::Unit* mineral_target = Pathfinder::findNearestMineralPatch(
                gAPI->observer().startingLocation());
            if (!mineral_target)
                break;

            Actions()->UnitCommand(unit_, sc2::ABILITY_ID::SMART, mineral_target);
            break;
        }

        case sc2::UNIT_TYPEID::TERRAN_BARRACKS:
        {
            m_trainingOrders.emplace_back(Observation()->GetUnitTypeData()[Convert::toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_MARINE)], unit_);
            break;
        }

        default:
        {
            break;
        }
    }
}
