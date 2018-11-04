// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Plugin.h"

struct WarpSmith : Plugin {
    WarpSmith();

    void OnStep() final;

    void OnUnitIdle(const sc2::Unit* unit_) final;

    void OnUpgradeCompleted(sc2::UpgradeID id_) final;

 private:
    bool m_warp_gates_researched;
};
