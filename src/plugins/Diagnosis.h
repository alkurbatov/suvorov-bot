// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#pragma once

#include "Builder.h"
#include "Plugin.h"

struct Diagnosis : Plugin {
    void OnStep(Builder*) final;

    void OnGameEnd() final;
};
