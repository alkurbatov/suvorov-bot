// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "objects/Geyser.h"
#include "objects/Worker.h"

#include <sc2api/sc2_gametypes.h>
#include <sc2api/sc2_unit.h>

#include <list>
#include <unordered_set>

struct Hub {
    explicit Hub(sc2::Race current_race_);

    void OnStep();

    void OnUnitCreated(const sc2::Unit& unit_);

    void OnUnitDestroyed(const sc2::Unit& unit_);

    void OnUnitIdle(const sc2::Unit& unit_);

    bool IsOccupied(const sc2::Unit& unit_) const;

    bool IsTargetOccupied(const sc2::UnitOrder& order_) const;

    void ClaimObject(const sc2::Unit& unit_);

    sc2::Race GetCurrentRace() const;

    Worker* GetClosestFreeWorker(const sc2::Point2D& location_);

    sc2::UNIT_TYPEID GetCurrentWorkerType() const;

    bool AssignRefineryConstruction(Order* order_, const sc2::Unit* geyser_);

    bool AssignBuildTask(Order* order_, const sc2::Point2D& point_);

    void AssignVespeneHarvester(const sc2::Unit& refinery_);

 private:
    sc2::Race m_current_race;
    sc2::UNIT_TYPEID m_current_worker_type;

    std::unordered_set<Geyser, GameObjectHasher> m_captured_geysers;

    std::list<Worker> m_busy_workers;
    std::list<Worker> m_free_workers;
};

extern std::unique_ptr<Hub> gHub;
