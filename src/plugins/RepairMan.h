// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Builder.h"
#include "Plugin.h"

struct RepairMan : Plugin {
    void OnStep(Builder* builder_) final;

    void OnUnitDestroyed(const sc2::Unit* unit_, Builder* builder_) final;
};
