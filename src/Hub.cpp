// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "Helpers.h"
#include "Historican.h"
#include "Hub.h"

Hub::Hub(sc2::Race current_race_): m_current_race(current_race_),
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
            m_free_workers.emplace_back(unit_);
            return;

        case sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR:
        case sc2::UNIT_TYPEID::TERRAN_REFINERY:
        case sc2::UNIT_TYPEID::ZERG_EXTRACTOR: {
            Geyser obj(unit_);

            auto it = std::find(
                m_captured_geysers.begin(), m_captured_geysers.end(), obj);
            if (m_captured_geysers.end() != it) {
                m_captured_geysers.erase(it);  // might be claimed geyser
                gHistory << "[INFO] " << "Release claimed geyser " <<
                    sc2::UnitTypeToName(unit_.unit_type) << std::endl;
            }

            m_captured_geysers.insert(obj);
            gHistory << "[INFO] " << "Capture object " <<
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
            auto it = std::find(m_busy_workers.begin(), m_busy_workers.end(), unit_);
            if (m_busy_workers.end() != it) {
                m_busy_workers.erase(it);
                gHistory << "[INFO] Our busy worker was destroyed" << std::endl;
                return;
            }

            it = std::find(m_free_workers.begin(), m_free_workers.end(), unit_);
            if (m_free_workers.end() != it) {
                m_free_workers.erase(it);
                gHistory << "[INFO] Our worker was destroyed" << std::endl;
                return;
            }

            return;
        }

        case sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR:
        case sc2::UNIT_TYPEID::TERRAN_REFINERY:
        case sc2::UNIT_TYPEID::ZERG_EXTRACTOR: {
            auto it = m_captured_geysers.find(Geyser(unit_));

            if (m_captured_geysers.end() != it) {
                m_captured_geysers.erase(it);
                gHistory << "[INFO] Release object " <<
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
            auto it = std::find(m_busy_workers.begin(), m_busy_workers.end(), unit_);

            if (m_busy_workers.end() != it) {
                m_free_workers.emplace_back(*it);
                m_busy_workers.erase(it);
                gHistory << "[INFO] Our worker finished task" << std::endl;
            }

            return;
        }

        default:
            break;
    }
}

bool Hub::IsOccupied(const sc2::Unit& unit_) const {
    auto it = std::find(m_captured_geysers.begin(), m_captured_geysers.end(),
        Geyser(unit_));

    return m_captured_geysers.end() != it;
}

bool Hub::IsTargetOccupied(const sc2::UnitOrder& order_) const {
    if (m_captured_geysers.empty())
        return false;

    return m_captured_geysers.end() != m_captured_geysers.find(Geyser(order_));
}

void Hub::ClaimObject(const sc2::Unit& unit_) {
    if (IsGeyser()(unit_)) {
        m_captured_geysers.emplace(unit_);
        gHistory << "[INFO] " << "Claim object " <<
            sc2::UnitTypeToName(unit_.unit_type) << std::endl;
    }
}

sc2::Race Hub::GetCurrentRace() const {
    return m_current_race;
}

Worker* Hub::GetClosestFreeWorker(const sc2::Point2D& location_) {
    if (m_free_workers.empty())
        return nullptr;

    auto closest_worker = m_free_workers.end();
    float distance = std::numeric_limits<float>::max();

    for (auto it = m_free_workers.begin(); it != m_free_workers.end(); ++it) {
        float d = sc2::DistanceSquared2D(it->GetPos(), location_);

        if (d >= distance)
            continue;

        distance = d;
        closest_worker = it;
    }

    m_busy_workers.push_back(*closest_worker);
    m_free_workers.erase(closest_worker);

    return &m_busy_workers.back();
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

std::unique_ptr<Hub> gHub;
