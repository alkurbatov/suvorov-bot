// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#pragma once

#include "Plugin.h"

struct WarpSmith : Plugin {
    WarpSmith();

    void OnStep(Builder* builder_) final;

    void OnUnitIdle(const sc2::Unit* unit_, Builder*) final;

    void OnUpgradeCompleted(sc2::UpgradeID id_) final;

 private:
    bool m_warp_gates_researched;
};
