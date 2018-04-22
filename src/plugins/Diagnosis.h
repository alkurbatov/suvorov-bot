// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "../Builder.h"
#include "Plugin.h"

#include <memory>

struct Diagnosis : Plugin {
    explicit Diagnosis(std::shared_ptr<Builder> builder_);

    void OnStep() final;

    void OnGameEnd() final;

 private:
    std::weak_ptr<Builder> m_builder;
};
