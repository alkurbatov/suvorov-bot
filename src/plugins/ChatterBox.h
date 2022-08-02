// The MIT License (MIT)
//
// Copyright (c) 2017-2022 Alexander Kurbatov

#pragma once

#include "Plugin.h"

struct ChatterBox: Plugin {
    void OnGameStart(Builder*) final;

    void OnStep(Builder*) final;
};
