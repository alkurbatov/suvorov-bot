// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

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
