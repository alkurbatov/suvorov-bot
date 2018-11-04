// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#pragma once

#include "../Builder.h"
#include "Plugin.h"

#include <memory>

struct RepairMan : Plugin {
    explicit RepairMan(const std::shared_ptr<Builder>& builder_);

    void OnStep() final;

    void OnUnitDestroyed(const sc2::Unit* unit_) final;

 private:
    std::weak_ptr<Builder> m_builder;
};
