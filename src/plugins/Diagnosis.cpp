// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#include "Diagnosis.h"
#include "Hub.h"
#include "Historican.h"
#include "core/API.h"

#include <sc2api/sc2_map_info.h>

void Diagnosis::OnStep(Builder* builder_) {
    for (const auto& i : gAPI->observer().GetChatMessages()) {
        if (i.message == "gg") {
            gHistory.warning() << "The game was finished forcibly." << std::endl;
            gAPI->debug().EndGame();
            return;
        }

        if (i.message == "grids") {
            auto info = gAPI->observer().GameInfo();
            sc2::PathingGrid(info).Dump("./pathing_grid.txt");
            sc2::PlacementGrid(info).Dump("./placement_grid.txt");
            sc2::HeightMap(info).Dump("./height_map.txt");
        }
    }

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
