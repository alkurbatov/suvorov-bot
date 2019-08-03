// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#pragma once

#include "Historican.h"

#include <chrono>

struct Timer {
    void Start();

    void Finish();

 private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};
