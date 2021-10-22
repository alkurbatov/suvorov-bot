// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#include "Addon.h"
#include "Blueprint.h"
#include "Building.h"
#include "Creature.h"
#include "GateUnit.h"
#include "HighTechUnit.h"
#include "Mutation.h"
#include "Queen.h"
#include "Refinery.h"
#include "Research.h"
#include "TownHall.h"
#include "Unit.h"
#include "core/API.h"
#include "core/Errors.h"

std::shared_ptr<Blueprint> Blueprint::Plot(sc2::ABILITY_ID ability_) {
    switch (ability_) {
        case sc2::ABILITY_ID::BUILD_REFINERY:
        case sc2::ABILITY_ID::BUILD_EXTRACTOR:
        case sc2::ABILITY_ID::BUILD_ASSIMILATOR:
            return std::make_shared<Refinery>();

        case sc2::ABILITY_ID::MORPH_BANELING:
        case sc2::ABILITY_ID::MORPH_BROODLORD:
        case sc2::ABILITY_ID::MORPH_GREATERSPIRE:
        case sc2::ABILITY_ID::MORPH_HIVE:
        case sc2::ABILITY_ID::MORPH_LAIR:
        case sc2::ABILITY_ID::MORPH_LURKER:
        case sc2::ABILITY_ID::MORPH_ORBITALCOMMAND:
        case sc2::ABILITY_ID::MORPH_OVERSEER:
        case sc2::ABILITY_ID::MORPH_PLANETARYFORTRESS:
        case sc2::ABILITY_ID::MORPH_RAVAGER:
            return std::make_shared<Mutation>();

        case sc2::ABILITY_ID::TRAIN_ADEPT:
        case sc2::ABILITY_ID::TRAIN_DARKTEMPLAR:
        case sc2::ABILITY_ID::TRAIN_HIGHTEMPLAR:
        case sc2::ABILITY_ID::TRAIN_SENTRY:
        case sc2::ABILITY_ID::TRAIN_STALKER:
        case sc2::ABILITY_ID::TRAIN_ZEALOT:
            return std::make_shared<GateUnit>();

        case sc2::ABILITY_ID::TRAINWARP_ADEPT:
        case sc2::ABILITY_ID::TRAINWARP_DARKTEMPLAR:
        case sc2::ABILITY_ID::TRAINWARP_HIGHTEMPLAR:
        case sc2::ABILITY_ID::TRAINWARP_SENTRY:
        case sc2::ABILITY_ID::TRAINWARP_STALKER:
        case sc2::ABILITY_ID::TRAINWARP_ZEALOT:
            // NOTE (alkurbatov): Instead please use
            // TRAIN_ADEPT, TRAIN_DARKTEMPLAR etc.
            throw InvalidBuildCommand(ability_);

        case sc2::ABILITY_ID::TRAIN_COLOSSUS:
        case sc2::ABILITY_ID::TRAIN_DISRUPTOR:
        case sc2::ABILITY_ID::TRAIN_IMMORTAL:
        case sc2::ABILITY_ID::TRAIN_OBSERVER:
        case sc2::ABILITY_ID::TRAIN_WARPPRISM:
            return std::make_shared<Unit>(sc2::UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY);

        case sc2::ABILITY_ID::TRAIN_CARRIER:
        case sc2::ABILITY_ID::TRAIN_ORACLE:
        case sc2::ABILITY_ID::TRAIN_PHOENIX:
        case sc2::ABILITY_ID::TRAIN_TEMPEST:
        case sc2::ABILITY_ID::TRAIN_VOIDRAY:
            return std::make_shared<Unit>(sc2::UNIT_TYPEID::PROTOSS_STARGATE);

        case sc2::ABILITY_ID::TRAIN_MOTHERSHIP:
            return std::make_shared<Unit>(sc2::UNIT_TYPEID::PROTOSS_NEXUS);

        case sc2::ABILITY_ID::TRAIN_BANSHEE:
        case sc2::ABILITY_ID::TRAIN_BATTLECRUISER:
        case sc2::ABILITY_ID::TRAIN_RAVEN:
            return std::make_shared<HighTechUnit>(
                sc2::UNIT_TYPEID::TERRAN_STARPORT,
                sc2::UNIT_TYPEID::TERRAN_STARPORTTECHLAB);

        case sc2::ABILITY_ID::TRAIN_LIBERATOR:
        case sc2::ABILITY_ID::TRAIN_MEDIVAC:
        case sc2::ABILITY_ID::TRAIN_VIKINGFIGHTER:
            return std::make_shared<Unit>(sc2::UNIT_TYPEID::TERRAN_STARPORT);

        case sc2::ABILITY_ID::TRAIN_HELLION:
        case sc2::ABILITY_ID::TRAIN_HELLBAT:
        case sc2::ABILITY_ID::TRAIN_WIDOWMINE:
            return std::make_shared<Unit>(sc2::UNIT_TYPEID::TERRAN_FACTORY);

        case sc2::ABILITY_ID::TRAIN_SIEGETANK:
        case sc2::ABILITY_ID::TRAIN_THOR:
        case sc2::ABILITY_ID::TRAIN_CYCLONE:
          return std::make_shared<HighTechUnit>(
                sc2::UNIT_TYPEID::TERRAN_FACTORY,
                sc2::UNIT_TYPEID::TERRAN_FACTORYTECHLAB);

        case sc2::ABILITY_ID::TRAIN_MARINE:
        case sc2::ABILITY_ID::TRAIN_REAPER:
            return std::make_shared<Unit>(sc2::UNIT_TYPEID::TERRAN_BARRACKS);

        case sc2::ABILITY_ID::TRAIN_GHOST:
        case sc2::ABILITY_ID::TRAIN_MARAUDER:
            return std::make_shared<HighTechUnit>(
                sc2::UNIT_TYPEID::TERRAN_BARRACKS,
                sc2::UNIT_TYPEID::TERRAN_BARRACKSTECHLAB);

        case sc2::ABILITY_ID::TRAIN_QUEEN:
            return std::make_shared<Queen>();

        case sc2::ABILITY_ID::TRAIN_PROBE:
        case sc2::ABILITY_ID::TRAIN_SCV:
            return std::make_shared<Unit>(sc2::UNIT_TYPEID::INVALID);

        case sc2::ABILITY_ID::TRAIN_DRONE:
        case sc2::ABILITY_ID::TRAIN_CORRUPTOR:
        case sc2::ABILITY_ID::TRAIN_INFESTOR:
        case sc2::ABILITY_ID::TRAIN_HYDRALISK:
        case sc2::ABILITY_ID::TRAIN_MUTALISK:
        case sc2::ABILITY_ID::TRAIN_OVERLORD:
        case sc2::ABILITY_ID::TRAIN_ROACH:
        case sc2::ABILITY_ID::TRAIN_SWARMHOST:
        case sc2::ABILITY_ID::TRAIN_VIPER:
        case sc2::ABILITY_ID::TRAIN_ULTRALISK:
        case sc2::ABILITY_ID::TRAIN_ZERGLING:
            return std::make_shared<Creature>();

        case sc2::ABILITY_ID::RESEARCH_WARPGATE:
            return std::make_shared<Research>(sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE);

        case sc2::ABILITY_ID::RESEARCH_COMBATSHIELD:
        case sc2::ABILITY_ID::RESEARCH_CONCUSSIVESHELLS:
        case sc2::ABILITY_ID::RESEARCH_STIMPACK:
            return std::make_shared<Research>(sc2::UNIT_TYPEID::TERRAN_BARRACKSTECHLAB);

        case sc2::ABILITY_ID::RESEARCH_HISECAUTOTRACKING:
        case sc2::ABILITY_ID::RESEARCH_NEOSTEELFRAME:
        case sc2::ABILITY_ID::RESEARCH_TERRANINFANTRYARMORLEVEL1:
        case sc2::ABILITY_ID::RESEARCH_TERRANINFANTRYARMORLEVEL2:
        case sc2::ABILITY_ID::RESEARCH_TERRANINFANTRYARMORLEVEL3:
        case sc2::ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONSLEVEL1:
        case sc2::ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONSLEVEL2:
        case sc2::ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONSLEVEL3:
        case sc2::ABILITY_ID::RESEARCH_TERRANSTRUCTUREARMORUPGRADE:
            return std::make_shared<Research>(sc2::UNIT_TYPEID::TERRAN_ENGINEERINGBAY);

        case sc2::ABILITY_ID::RESEARCH_TERRANSHIPWEAPONSLEVEL1:
        case sc2::ABILITY_ID::RESEARCH_TERRANSHIPWEAPONSLEVEL2:
        case sc2::ABILITY_ID::RESEARCH_TERRANSHIPWEAPONSLEVEL3:
        case sc2::ABILITY_ID::RESEARCH_TERRANVEHICLEANDSHIPPLATINGLEVEL1:
        case sc2::ABILITY_ID::RESEARCH_TERRANVEHICLEANDSHIPPLATINGLEVEL2:
        case sc2::ABILITY_ID::RESEARCH_TERRANVEHICLEANDSHIPPLATINGLEVEL3:
        case sc2::ABILITY_ID::RESEARCH_TERRANVEHICLEWEAPONSLEVEL1:
        case sc2::ABILITY_ID::RESEARCH_TERRANVEHICLEWEAPONSLEVEL2:
        case sc2::ABILITY_ID::RESEARCH_TERRANVEHICLEWEAPONSLEVEL3:
            return std::make_shared<Research>(sc2::UNIT_TYPEID::TERRAN_ARMORY);

        case sc2::ABILITY_ID::RESEARCH_BATTLECRUISERWEAPONREFIT:
            return std::make_shared<Research>(sc2::UNIT_TYPEID::TERRAN_FUSIONCORE);

        case sc2::ABILITY_ID::RESEARCH_PERSONALCLOAKING:
            return std::make_shared<Research>(sc2::UNIT_TYPEID::TERRAN_GHOSTACADEMY);

        case sc2::ABILITY_ID::RESEARCH_PROTOSSGROUNDARMORLEVEL1:
        case sc2::ABILITY_ID::RESEARCH_PROTOSSGROUNDARMORLEVEL2:
        case sc2::ABILITY_ID::RESEARCH_PROTOSSGROUNDARMORLEVEL3:
        case sc2::ABILITY_ID::RESEARCH_PROTOSSGROUNDWEAPONSLEVEL1:
        case sc2::ABILITY_ID::RESEARCH_PROTOSSGROUNDWEAPONSLEVEL2:
        case sc2::ABILITY_ID::RESEARCH_PROTOSSGROUNDWEAPONSLEVEL3:
        case sc2::ABILITY_ID::RESEARCH_PROTOSSSHIELDSLEVEL1:
        case sc2::ABILITY_ID::RESEARCH_PROTOSSSHIELDSLEVEL2:
        case sc2::ABILITY_ID::RESEARCH_PROTOSSSHIELDSLEVEL3:
            return std::make_shared<Research>(sc2::UNIT_TYPEID::PROTOSS_FORGE);

        case sc2::ABILITY_ID::RESEARCH_ADEPTRESONATINGGLAIVES:
        case sc2::ABILITY_ID::RESEARCH_BLINK:
        case sc2::ABILITY_ID::RESEARCH_CHARGE:
            return std::make_shared<Research>(sc2::UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL);

        case sc2::ABILITY_ID::BUILD_COMMANDCENTER:
        case sc2::ABILITY_ID::BUILD_HATCHERY:
        case sc2::ABILITY_ID::BUILD_NEXUS:
            return std::make_shared<TownHall>();

        case sc2::ABILITY_ID::RESEARCH_ZERGLINGADRENALGLANDS:
        case sc2::ABILITY_ID::RESEARCH_ZERGLINGMETABOLICBOOST:
            return std::make_shared<Research>(sc2::UNIT_TYPEID::ZERG_SPAWNINGPOOL);

        case sc2::ABILITY_ID::RESEARCH_ZERGGROUNDARMORLEVEL1:
        case sc2::ABILITY_ID::RESEARCH_ZERGGROUNDARMORLEVEL2:
        case sc2::ABILITY_ID::RESEARCH_ZERGGROUNDARMORLEVEL3:
        case sc2::ABILITY_ID::RESEARCH_ZERGMELEEWEAPONSLEVEL1:
        case sc2::ABILITY_ID::RESEARCH_ZERGMELEEWEAPONSLEVEL2:
        case sc2::ABILITY_ID::RESEARCH_ZERGMELEEWEAPONSLEVEL3:
        case sc2::ABILITY_ID::RESEARCH_ZERGMISSILEWEAPONSLEVEL1:
        case sc2::ABILITY_ID::RESEARCH_ZERGMISSILEWEAPONSLEVEL2:
        case sc2::ABILITY_ID::RESEARCH_ZERGMISSILEWEAPONSLEVEL3:
            return std::make_shared<Research>(sc2::UNIT_TYPEID::ZERG_EVOLUTIONCHAMBER);

        case sc2::ABILITY_ID::BUILD_REACTOR_BARRACKS:
        case sc2::ABILITY_ID::BUILD_TECHLAB_BARRACKS:
            return std::make_shared<Addon>(sc2::UNIT_TYPEID::TERRAN_BARRACKS);

        case sc2::ABILITY_ID::BUILD_REACTOR_FACTORY:
        case sc2::ABILITY_ID::BUILD_TECHLAB_FACTORY:
            return std::make_shared<Addon>(sc2::UNIT_TYPEID::TERRAN_FACTORY);

        case sc2::ABILITY_ID::BUILD_REACTOR_STARPORT:
        case sc2::ABILITY_ID::BUILD_TECHLAB_STARPORT:
            return std::make_shared<Addon>(sc2::UNIT_TYPEID::TERRAN_STARPORT);

        case sc2::ABILITY_ID::BUILD_REACTOR:
        case sc2::ABILITY_ID::BUILD_TECHLAB:
            // NOTE (alkurbatov): Instead please use
            // BUILD_REACTOR_BARRACKS, BUILD_TECHLAB_FACTORY etc.
            throw InvalidBuildCommand(ability_);

        default:
            return std::make_shared<Building>();
    }
}
