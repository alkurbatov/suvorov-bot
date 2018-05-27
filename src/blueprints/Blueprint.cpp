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
            return std::shared_ptr<Blueprint>(new Refinery());

        case sc2::ABILITY_ID::MORPH_ORBITALCOMMAND:
            return std::shared_ptr<Blueprint>(new Orbital());

        case sc2::ABILITY_ID::MORPH_PLANETARYFORTRESS:
            return std::shared_ptr<Blueprint>(new Fortress());

        case sc2::ABILITY_ID::TRAIN_SCV:
        case sc2::ABILITY_ID::TRAIN_MARINE:
        case sc2::ABILITY_ID::TRAIN_REAPER:
            return std::shared_ptr<Blueprint>(new Unit());

        case sc2::ABILITY_ID::TRAIN_DRONE:
        case sc2::ABILITY_ID::TRAIN_OVERLORD:
        case sc2::ABILITY_ID::TRAIN_ZERGLING:
            return std::shared_ptr<Blueprint>(new ZergUnit());

        default:
            return std::shared_ptr<Blueprint>(new Building());
    }
}
