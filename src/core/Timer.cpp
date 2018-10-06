// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "Historican.h"
#include "Timer.h"

void Timer::Start() {
    m_start = std::chrono::high_resolution_clock::now();
}

void Timer::Finish() {
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - m_start).count();

    if (duration < 100000)
        return;

    gHistory.warning() << "Time consumed: " << duration << " microseconds" << std::endl;
}
