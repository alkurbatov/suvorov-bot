// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "Blueprint.h"
#include "Building.h"
#include "Fortress.h"
#include "Orbital.h"
#include "Refinery.h"
#include "Unit.h"
#include "ZergUnit.h"

Blueprint::Blueprint(bool needs_worker_): m_needs_worker(needs_worker_) {
}

Blueprint::~Blueprint() {
}

bool Blueprint::NeedsWorker() const {
    return m_needs_worker;
}

std::shared_ptr<Blueprint> Blueprint::Plot(sc2::ABILITY_ID ability_) {
    switch (ability_) {
        case sc2::ABILITY_ID::BUILD_REFINERY:
        case sc2::ABILITY_ID::BUILD_EXTRACTOR:
        case sc2::ABILITY_ID::BUILD_ASSIMILATOR:
            return std::shared_ptr<Blueprint>(new Refinery());

        case sc2::ABILITY_ID::MORPH_ORBITALCOMMAND:
            return std::shared_ptr<Blueprint>(new Orbital());

        case sc2::ABILITY_ID::MORPH_PLANETARYFORTRESS:
            return std::shared_ptr<Blueprint>(new Fortress());

        case sc2::ABILITY_ID::TRAIN_BANSHEE:
        case sc2::ABILITY_ID::TRAIN_BATTLECRUISER:
        case sc2::ABILITY_ID::TRAIN_CYCLONE:
        case sc2::ABILITY_ID::TRAIN_HELLION:
        case sc2::ABILITY_ID::TRAIN_HELLBAT:
        case sc2::ABILITY_ID::TRAIN_LIBERATOR:
        case sc2::ABILITY_ID::TRAIN_MARAUDER:
        case sc2::ABILITY_ID::TRAIN_MARINE:
        case sc2::ABILITY_ID::TRAIN_MEDIVAC:
        case sc2::ABILITY_ID::TRAIN_REAPER:
        case sc2::ABILITY_ID::TRAIN_SCV:
        case sc2::ABILITY_ID::TRAIN_SIEGETANK:
        case sc2::ABILITY_ID::TRAIN_THOR:
        case sc2::ABILITY_ID::TRAIN_VIKINGFIGHTER:
        case sc2::ABILITY_ID::TRAIN_WIDOWMINE:
            return std::shared_ptr<Blueprint>(new Unit());

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
            return std::shared_ptr<Blueprint>(new ZergUnit());

        default:
            return std::shared_ptr<Blueprint>(new Building());
    }
}
