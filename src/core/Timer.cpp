// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

#include "Historican.h"
#include "Timer.h"

Timer::Timer(): m_start(std::chrono::high_resolution_clock::now()) {
}

Timer::~Timer() {
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - m_start).count();

    // NOTE (alkurbatov): The ladder allows up to 44.4ms for step processing.
    // Ideally, we shouldn't reach this threshold.
    // Higher values could lead to ban and make the game much harder to play
    // in realtime mode.
    if (duration > 40000) {
        Historican("timer").warning()
            << "Step processing took: " << duration << " mcs" << std::endl;
    }
}
