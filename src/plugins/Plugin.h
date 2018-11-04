// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_unit.h>

struct Plugin {
    virtual ~Plugin() {
    }

    virtual void OnGameStart() {
    }

    virtual void OnStep() = 0;

    virtual void OnUnitCreated(const sc2::Unit*) {
    }

    virtual void OnUnitDestroyed(const sc2::Unit*) {
    }

    virtual void OnUnitIdle(const sc2::Unit*) {
    }

    virtual void OnUpgradeCompleted(sc2::UpgradeID) {
    }

    virtual void OnGameEnd() {
    }
};
