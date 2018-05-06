// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "../Builder.h"
#include "Plugin.h"

#include <memory>

struct Governor : Plugin {
    explicit Governor(std::shared_ptr<Builder> builder_);

    void OnStep() final;

 private:
    std::weak_ptr<Builder> m_builder;
};
