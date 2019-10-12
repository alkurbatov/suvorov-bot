// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#pragma once

#include <chrono>

struct Timer {
    Timer();

    ~Timer();

 private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};
