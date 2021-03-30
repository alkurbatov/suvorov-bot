// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#include "Plugin.h"

Plugin::Plugin(): m_frequency(1) {
}

Plugin::Plugin(uint32_t frequency): m_frequency(frequency) {
}

bool Plugin::CheckFrequency(uint32_t count_) const {
    if (count_ % m_frequency == 0)
        return true;

    return false;
}
