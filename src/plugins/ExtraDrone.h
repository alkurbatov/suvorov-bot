// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#pragma once

#include "Builder.h"
#include "Plugin.h"

struct ExtraDrone : Plugin {
    ExtraDrone();

    void OnStep(Builder*) final;

    void OnUnitCreated(const sc2::Unit* unit_, Builder* builder_) final;

 private:
    enum State {
        WAIT_SUPPLY_CAP,
        WAIT_EXTRACTOR_CREATION,
        WAIT_WORKER_CREATION,
        FINISHED,
    };

    State m_state;
    sc2::Tag m_extractor;
};
