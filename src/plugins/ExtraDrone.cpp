// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

#include "ExtraDrone.h"
#include "core/API.h"
#include "core/Helpers.h"

#include <sc2api/sc2_typeenums.h>

ExtraDrone::ExtraDrone(): m_state(State::WAIT_SUPPLY_CAP) {
}

void ExtraDrone::OnStep(Builder*) {
    if (m_state != State::WAIT_WORKER_CREATION)
        return;

    auto eggs = gAPI->observer().GetUnits(IsUnit(sc2::UNIT_TYPEID::ZERG_EGG));
    if (!eggs.HasOrder(sc2::ABILITY_ID::TRAIN_DRONE))
        return;

    const auto extractor = gAPI->observer().GetUnit(m_extractor);
    gAPI->action().CancelConstruction(*extractor);
    m_state = State::FINISHED;
}

void ExtraDrone::OnUnitCreated(const sc2::Unit* unit_, Builder* builder_) {
    if (m_state == State::WAIT_SUPPLY_CAP) {
        if (gAPI->observer().CountUnitType(sc2::UNIT_TYPEID::ZERG_DRONE) < 14)
            return;

        builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::ZERG_DRONE, true);
        builder_->ScheduleObligatoryOrder(sc2::UNIT_TYPEID::ZERG_EXTRACTOR, true);
        m_state = State::WAIT_EXTRACTOR_CREATION;
        return;
    }

    if (m_state == State::WAIT_EXTRACTOR_CREATION) {
        if (unit_->unit_type != sc2::UNIT_TYPEID::ZERG_EXTRACTOR)
            return;

        m_extractor = unit_->tag;
        m_state = State::WAIT_WORKER_CREATION;
    }
}
