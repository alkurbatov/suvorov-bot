// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

#pragma once

#include "Builder.h"
#include "Plugin.h"

#include <list>

struct RepairMan : Plugin {
    void OnStep(Builder* builder_) final;

    void OnUnitDamaged(const sc2::Unit* unit_, float health_, float shields_, Builder*) final;

    void OnUnitDestroyed(const sc2::Unit* unit_, Builder* builder_) final;

 private:
    void AssignRepairTask(const sc2::Unit& damaged_);

    std::list<const sc2::Unit*> m_damaged_buildings;
};
