// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "Diagnosis.h"
#include "Hub.h"
#include "Historican.h"
#include "core/API.h"

void Diagnosis::OnStep(Builder* builder_) {
    gAPI->debug().DrawText("Build order:");

    if (builder_->GetConstructionOrders().empty()) {
        gAPI->debug().DrawText("Empty");
    } else {
        for (const auto& i : builder_->GetConstructionOrders())
            gAPI->debug().DrawText(i.name);
    }

    for (const auto& i : gHub->GetExpansions())
        gAPI->debug().DrawSphere(i.town_hall_location, 0.35f);

    gAPI->debug().SendDebug();
}

void Diagnosis::OnGameEnd() {
    gAPI->control().SaveReplay();
    gHistory.info() << "Replay saved" << std::endl;
}
