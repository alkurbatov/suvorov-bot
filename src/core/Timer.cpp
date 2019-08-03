// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#include "Historican.h"
#include "Timer.h"

void Timer::Start() {
    m_start = std::chrono::high_resolution_clock::now();
}

void Timer::Finish() {
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - m_start).count();

    // NOTE (alkurbatov): 60ms is the maximum step processing time allowed
    // by the ladder. Higher values could lead to ban.
    if (duration > 60000) {
        gHistory.error() << "Step processing took: " << duration << " mcs" << std::endl;
        return;
    }

    // NOTE (alkurbatov): The ladder allows up to 44.4ms for step processing.
    // Ideally, we shouldn't reach this threshold.
    if (duration > 44400) {
        gHistory.warning() << "Step processing took: " << duration << " mcs" << std::endl;
        return;
    }
}
