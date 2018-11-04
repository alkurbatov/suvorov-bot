// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "Historican.h"
#include "Hub.h"
#include "core/Helpers.h"

Hub::Hub(sc2::Race current_race_, const Expansions& expansions_):
    m_current_race(current_race_), m_expansions(expansions_),
    m_current_worker_type(sc2::UNIT_TYPEID::INVALID) {
    switch (m_current_race) {
        case sc2::Race::Protoss:
            m_current_worker_type = sc2::UNIT_TYPEID::PROTOSS_PROBE;
            return;

        case sc2::Race::Terran:
            m_current_worker_type = sc2::UNIT_TYPEID::TERRAN_SCV;
            return;

        case sc2::Race::Zerg:
            m_current_worker_type = sc2::UNIT_TYPEID::ZERG_DRONE;
            return;

        default:
            return;
    }
}

void Hub::OnStep() {
}

void Hub::OnUnitCreated(const sc2::Unit& unit_) {
    switch (unit_.unit_type.ToType()) {
        case sc2::UNIT_TYPEID::PROTOSS_PROBE:
        case sc2::UNIT_TYPEID::TERRAN_SCV:
        case sc2::UNIT_TYPEID::ZERG_DRONE:
            m_free_workers.Add(Worker(unit_));
            return;

        case sc2::UNIT_TYPEID::ZERG_LARVA:
            m_larva.Add(GameObject(unit_));
            return;

        case sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR:
        case sc2::UNIT_TYPEID::TERRAN_REFINERY:
        case sc2::UNIT_TYPEID::ZERG_EXTRACTOR: {
            Geyser obj(unit_);

            if (m_captured_geysers.Remove(obj))  // might be claimed geyser
                gHistory.info() << "Release claimed geyser " << std::endl;

            m_captured_geysers.Add(obj);
            gHistory.info() << "Capture object " <<
                sc2::UnitTypeToName(unit_.unit_type) << std::endl;
            return;
        }

        default:
            return;
    }
}

void Hub::OnUnitDestroyed(const sc2::Unit& unit_) {
    switch (unit_.unit_type.ToType()) {
        case sc2::UNIT_TYPEID::PROTOSS_PROBE:
        case sc2::UNIT_TYPEID::TERRAN_SCV:
        case sc2::UNIT_TYPEID::ZERG_DRONE: {
            if (m_busy_workers.Remove(Worker(unit_))) {
                gHistory.info() << "Our busy worker was destroyed" << std::endl;
                return;
            }

            if (m_free_workers.Remove(Worker(unit_)))
                gHistory.info() << "Our free worker was destroyed" << std::endl;

            return;
        }

        case sc2::UNIT_TYPEID::ZERG_LARVA:
            if (m_larva.Remove(GameObject(unit_)))
                gHistory.info() << "Our larva was destroyed" << std::endl;

            return;

        case sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR:
        case sc2::UNIT_TYPEID::TERRAN_REFINERY:
        case sc2::UNIT_TYPEID::ZERG_EXTRACTOR: {
            if (m_captured_geysers.Remove(Geyser(unit_))) {
                gHistory.info() << "Release object " <<
                    sc2::UnitTypeToName(unit_.unit_type) << std::endl;
            }

            return;
        }

        default:
            return;
    }
}

void Hub::OnUnitIdle(const sc2::Unit& unit_) {
    switch (unit_.unit_type.ToType()) {
        case sc2::UNIT_TYPEID::PROTOSS_PROBE:
        case sc2::UNIT_TYPEID::TERRAN_SCV:
        case sc2::UNIT_TYPEID::ZERG_DRONE: {
            if (m_free_workers.Swap(Worker(unit_), m_busy_workers))
                gHistory.info() << "Our busy worker has finished task" << std::endl;

            return;
        }

        case sc2::UNIT_TYPEID::ZERG_LARVA: {
            GameObject obj = GameObject(unit_);
            if (!m_larva.IsCached(obj)) {
                m_larva.Add(obj);
                gHistory.info() << "Picked up an idle larva." << std::endl;
            }

            return;
        }

        default:
            break;
    }
}

bool Hub::IsOccupied(const sc2::Unit& unit_) const {
    return m_captured_geysers.IsCached(Geyser(unit_));
}

bool Hub::IsTargetOccupied(const sc2::UnitOrder& order_) const {
    return m_captured_geysers.IsCached(Geyser(order_));
}

void Hub::ClaimObject(const sc2::Unit& unit_) {
    if (IsVisibleGeyser()(unit_)) {
        m_captured_geysers.Add(Geyser(unit_));
        gHistory.info() << "Claim object " <<
            sc2::UnitTypeToName(unit_.unit_type) << std::endl;
    }
}

sc2::Race Hub::GetCurrentRace() const {
    return m_current_race;
}

Worker* Hub::GetClosestFreeWorker(const sc2::Point2D& location_) {
    Worker* closest_worker = m_free_workers.GetClosestTo(location_);
    if (!closest_worker)
        return nullptr;

    m_free_workers.Swap(*closest_worker, m_busy_workers);
    return &m_busy_workers.Back();
}

sc2::UNIT_TYPEID Hub::GetCurrentWorkerType() const {
    return m_current_worker_type;
}

bool Hub::AssignRefineryConstruction(Order* order_, const sc2::Unit* geyser_) {
    Worker* worker = GetClosestFreeWorker(geyser_->pos);
    if (!worker)
        return false;

    worker->BuildRefinery(order_, geyser_);
    ClaimObject(*geyser_);
    return true;
}

bool Hub::AssignBuildTask(Order* order_, const sc2::Point2D& point_) {
    Worker* worker = GetClosestFreeWorker(point_);
    if (!worker)
        return false;

    worker->Build(order_, point_);
    return true;
}

void Hub::AssignVespeneHarvester(const sc2::Unit& refinery_) {
    Worker* worker = GetClosestFreeWorker(refinery_.pos);
    if (!worker)
        return;

    worker->GatherVespene(refinery_);
}

bool Hub::AssignLarva(Order* order_) {
    if (m_larva.Empty())
        return false;

    order_->assignee = m_larva.Back().Tag();
    gAPI->action().Build(*order_);

    m_larva.PopBack();
    return true;
}

const Cache<GameObject>&  Hub::GetLarvas() const {
    return m_larva;
}

const Expansions& Hub::GetExpansions() const {
    return m_expansions;
}

std::unique_ptr<Hub> gHub;
