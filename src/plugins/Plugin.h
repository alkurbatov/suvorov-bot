// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "Builder.h"

#include <sc2api/sc2_unit.h>

struct Plugin {
    virtual ~Plugin() {
    }

    virtual void OnGameStart(Builder*) {
    }

    virtual void OnStep(Builder*) {
    }

    virtual void OnUnitCreated(const sc2::Unit*, Builder*) {
    }

    virtual void OnUnitDestroyed(const sc2::Unit*, Builder*) {
    }

    virtual void OnUnitIdle(const sc2::Unit*, Builder*) {
    }

    virtual void OnUpgradeCompleted(sc2::UpgradeID) {
    }

    virtual void OnGameEnd() {
    }
};
