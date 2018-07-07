// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "../Builder.h"
#include "Plugin.h"

#include <memory>

struct QuarterMaster : Plugin {
    explicit QuarterMaster(const std::shared_ptr<Builder>& builder_);

    void OnStep() final;

    void OnUnitCreated(const sc2::Unit* unit_) final;

 private:
    bool m_skip_turn;
    std::weak_ptr<Builder> m_builder;
};
