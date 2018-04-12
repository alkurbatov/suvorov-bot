// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "Converter.h"
#include "Dispatcher.h"
#include "Historican.h"
#include "Pathfinder.h"
#include "Timer.h"

#include <sc2api/sc2_common.h>
#include <sc2api/sc2_unit.h>

Dispatcher::Dispatcher() {
    gAPI.reset(new API::Interface(Actions(), Control(), Debug(), Observation(), Query()));
}

void Dispatcher::OnGameStart() {
    gHistory << "New Game started!" << std::endl;

    // Initial build order
    ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_REFINERY);
    ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND);
    ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_BARRACKS);
    ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
    ScheduleConstruction(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT);

    m_chatterbox.OnGameStart();
}

void Dispatcher::OnGameEnd() {
    gHistory << "Game over!" <<std::endl;

    m_diagnosis.OnGameEnd();
}

void Dispatcher::OnBuildingConstructionComplete(const sc2::Unit* building_) {
    gHistory << "Loop Step #" << Observation()->GetGameLoop() <<
        ": Building was created, tag: " << building_->tag << std::endl;
}

void Dispatcher::OnStep() {
    Timer clock;
    clock.Start();

    m_builder.OnStep();
    m_chatterbox.OnStep();
    m_force_commander.OnStep();

    auto it = m_construction_orders.begin();
    while (it != m_construction_orders.end()) {
        if (!m_builder.Build(&(*it)))
            break;

        it = m_construction_orders.erase(it);
    }

    it = m_training_orders.begin();
    while (it != m_training_orders.end()) {
        if (!m_builder.Build(&(*it))) {
            ++it;
            continue;
        }

        it = m_training_orders.erase(it);
    }

    // FIXME: skip this if we've planned additional supply already.
    // FIXME: skip this if we have 200 cap limit.
    // If we are not supply capped, don't build a supply depot.
    // size_t prediction = gAPI->observer().countUnitType(
    //     sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT) * 2 + 2;
    // if (Observation()->GetFoodUsed() <=
    //     Observation()->GetFoodCap() - static_cast<int32_t>(prediction))
    //     return;

    // m_constructionOrders.emplace(Observation()->GetUnitTypeData()[
    //     toUnitTypeID(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT)]);

    m_diagnosis.ShowBuildOrder(m_construction_orders);
    m_diagnosis.OnStep();

    clock.Finish();
}

void Dispatcher::OnUnitCreated(const sc2::Unit* unit_) {
    gHistory << "Loop Step #" << Observation()->GetGameLoop() <<
        ": Unit was created, tag: " << unit_->tag << std::endl;

    m_force_commander.OnUnitCreated(unit_);
}

void Dispatcher::OnUnitIdle(const sc2::Unit* unit_) {
    switch (unit_->unit_type.ToType()) {
        case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER:
        case sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND:
        case sc2::UNIT_TYPEID::TERRAN_PLANETARYFORTRESS: {
            // If we can add more SCVs do it.
            if (unit_->assigned_harvesters < unit_->ideal_harvesters) {
                m_training_orders.emplace_back(Observation()->GetUnitTypeData()[
                    convert::ToUnitTypeID(sc2::UNIT_TYPEID::TERRAN_SCV)], unit_);
            }

            break;
        }

        case sc2::UNIT_TYPEID::TERRAN_SCV: {
            const sc2::Unit* mineral_target = Pathfinder::FindMineralPatch(
                gAPI->observer().StartingLocation());
            if (!mineral_target)
                break;

            Actions()->UnitCommand(unit_, sc2::ABILITY_ID::SMART, mineral_target);
            break;
        }

        case sc2::UNIT_TYPEID::TERRAN_BARRACKS: {
            m_training_orders.emplace_back(Observation()->GetUnitTypeData()[
                convert::ToUnitTypeID(sc2::UNIT_TYPEID::TERRAN_MARINE)], unit_);
            break;
        }

        default: {
            break;
        }
    }
}

void Dispatcher::ScheduleConstruction(sc2::UNIT_TYPEID id_) {
    auto& data = Observation()->GetUnitTypeData();

    sc2::UnitTypeData structure = data[convert::ToUnitTypeID(id_)];

    // NOTE(alkurbatov): Unfortunally SC2 API returns wrong mineral cost
    // for orbital command and planetary fortress so we use a workaround.
    // See https://github.com/Blizzard/s2client-api/issues/191
    if (id_ == sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND ||
        id_ == sc2::UNIT_TYPEID::TERRAN_PLANETARYFORTRESS) {
            structure.mineral_cost = 150;
    }

    m_construction_orders.emplace_back(structure);
}
