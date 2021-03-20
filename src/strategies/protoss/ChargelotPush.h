// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#pragma once

#include "Builder.h"
#include "strategies/Strategy.h"

struct ChargelotPush : Strategy {
    ChargelotPush();

    void OnGameStart(Builder* builder_) final;

    void OnUpgradeCompleted(sc2::UpgradeID) final;

    void OnUnitIdle(const sc2::Unit* unit_, Builder* builder_) final;

 private:
    bool m_build_zealots;
};
