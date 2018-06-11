// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "../Builder.h"
#include "Plugin.h"

#include <memory>

struct Miner : Plugin {
    explicit Miner(const std::shared_ptr<Builder>& builder_);

    void OnStep() final;

    void OnUnitIdle(const sc2::Unit* unit_) final;

 private:
    std::weak_ptr<Builder> m_builder;
};
