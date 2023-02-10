// The MIT License (MIT)
//
// Copyright (c) 2017-2022 Alexander Kurbatov

#include "Worker.h"
#include "core/API.h"

Worker::Worker(const sc2::Unit& unit_):
    GameObject(unit_), m_job(Job::GATHERING_MINERALS) {
}

void Worker::BuildRefinery(Order* order_, const sc2::Unit* geyser_) {
    order_->assignee = Tag();

    gAPI->action().Build(*order_, geyser_);
    m_job = Job::BUILDING_REFINERY;
}

void Worker::Build(Order* order_, const sc2::Point2D& point_) {
    order_->assignee = Tag();

    gAPI->action().Build(*order_, point_);
    m_job = Job::BUILDING;
}

void Worker::GatherVespene(const sc2::Unit& target_) {
    gAPI->action().Cast(ToUnit(), sc2::ABILITY_ID::SMART, target_);
    m_job = Job::GATHERING_VESPENE;
}

void Worker::Repair(const sc2::Unit& target_) {
    // NOTE (impulsecloud): STOP first, then queue REPAIR to avoid bug
    // when Minerals drop to 0 unexpectedly, REPAIR commands fail & keep GATHERING
    // this bypasses OnIdle and leaves worker in BUSY list otherwise
    gAPI->action().Cast(ToUnit(), sc2::ABILITY_ID::STOP_STOP);
    gAPI->action().Cast(ToUnit(), sc2::ABILITY_ID::EFFECT_REPAIR, target_, true);
    gAPI->action().ToggleAutocast(Tag(), sc2::ABILITY_ID::EFFECT_REPAIR_SCV);
    m_job = Job::REPAIRING;
}

void Worker::Attack(const sc2::Unit& target_) {
    gAPI->action().Cast(ToUnit(), sc2::ABILITY_ID::SMART, target_);
    m_job = Job::ATTACKING;
}
