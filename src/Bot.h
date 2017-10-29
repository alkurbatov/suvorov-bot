#include <sc2api/sc2_unit.h>
#include <sc2api/sc2_agent.h>
#include <sc2api/sc2_common.h>
#include <sc2api/sc2_map_info.h>
#include <sc2api/sc2_typeenums.h>
#include <sc2api/sc2_interfaces.h>

#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>

#include <iostream>

using namespace boost::log::trivial;

// The main bot class.
struct Bot: sc2::Agent
{

virtual void OnGameStart() final
{
    BOOST_LOG_SEV(m_logger, info) << "New Game started!";

    m_startLocation = Observation()->GetStartLocation();
}

virtual void OnStep() final {
    TryBuildSupplyDepot();
    TryBuildBarracks();
}

virtual void OnUnitCreated(const sc2::Unit* unit) final {
    BOOST_LOG_SEV(m_logger, info) <<
        "Loop Step #" << Observation()->GetGameLoop() <<
        ": Unit was created, tag: " << unit->tag;
}

virtual void OnUnitIdle(const sc2::Unit* unit) final {
    switch (unit->unit_type.ToType()) {
        case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER: {
            // unit->assigned_harvesters and unit->ideal_harvesters are zeroes
            // on game start but we need to start building SCVs anyways.
            if (unit->assigned_harvesters == 0 ||
                    unit->assigned_harvesters < unit->ideal_harvesters) {
                Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_SCV);
            }

            break;
        }

        case sc2::UNIT_TYPEID::TERRAN_SCV: {
            const sc2::Unit* mineral_target = FindNearestMineralPatch(m_startLocation);
            if (!mineral_target)
                break;

            Actions()->UnitCommand(unit, sc2::ABILITY_ID::SMART, mineral_target);
            break;
        }

        case sc2::UNIT_TYPEID::TERRAN_BARRACKS: {
            Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_MARINE);
            break;
        }

        case sc2::UNIT_TYPEID::TERRAN_MARINE: {
           if (CountUnitType(sc2::UNIT_TYPEID::TERRAN_MARINE) < 13)
                break;

            const sc2::GameInfo& game_info = Observation()->GetGameInfo();
            Actions()->UnitCommand(unit, sc2::ABILITY_ID::ATTACK_ATTACK, game_info.enemy_start_locations.front());
            break;
        }

        default: {
            break;
        }
    }
}

private:

void TryBuildStructure(sc2::ABILITY_ID ability_type_for_structure) {
    const sc2::ObservationInterface* observation = Observation();

    // If a unit already is building a supply structure of this type, do nothing.
    // Also get an scv to build the structure.
    const sc2::Unit* unit_to_build = nullptr;
    sc2::Units units = observation->GetUnits(sc2::Unit::Alliance::Self);
    for (const auto& unit : units) {
        for (const auto& order : unit->orders) {
            if (order.ability_id == ability_type_for_structure) {
                return;
            }
        }

        if (unit->unit_type ==  sc2::UNIT_TYPEID::TERRAN_SCV) {
            unit_to_build = unit;
        }
    }

    if (!unit_to_build)
        return;

    float rx = sc2::GetRandomScalar();
    float ry = sc2::GetRandomScalar();

    Actions()->UnitCommand(unit_to_build,
        ability_type_for_structure,
        sc2::Point2D(unit_to_build->pos.x + rx * 15.0f, unit_to_build->pos.y + ry * 15.0f));

    return;
}

void TryBuildSupplyDepot() {
    const sc2::ObservationInterface* observation = Observation();
    size_t prediction = CountUnitType(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT) * 2 + 2;

    // If we are not supply capped, don't build a supply depot.
    if (observation->GetFoodUsed() <= observation->GetFoodCap() - static_cast<int32_t>(prediction))
        return;

    // Try and build a depot. Find a random SCV and give it the order.
    TryBuildStructure(sc2::ABILITY_ID::BUILD_SUPPLYDEPOT);
}

const sc2::Unit* FindNearestMineralPatch(const sc2::Point2D& start) {
    const sc2::Unit* target = nullptr;
    float distance = std::numeric_limits<float>::max();
    sc2::Units units = Observation()->GetUnits(sc2::Unit::Alliance::Neutral);

    for (const auto& u : units) {
        if (u->unit_type != sc2::UNIT_TYPEID::NEUTRAL_MINERALFIELD)
            continue;

        float d = sc2::DistanceSquared2D(u->pos, start);
        if (d < distance) {
            distance = d;
            target = u;
        }
    }

    return target;
}

size_t CountUnitType(sc2::UNIT_TYPEID unit_type) {
    return Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsUnit(unit_type)).size();
}

void TryBuildBarracks() {
    if (CountUnitType(sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT) < 1) {
        return;
    }

    if (CountUnitType(sc2::UNIT_TYPEID::TERRAN_BARRACKS) > 3) {
        return;
    }

    TryBuildStructure(sc2::ABILITY_ID::BUILD_BARRACKS);
}

private:
    sc2::Point3D m_startLocation;

    boost::log::sources::severity_logger<severity_level> m_logger;
};
