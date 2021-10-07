// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#include "Diagnosis.h"
#include "Hub.h"
#include "Historican.h"
#include "core/API.h"

#include <sc2api/sc2_map_info.h>

namespace {
Historican gHistory("plugin.diagnosis");
}  // namespace

Diagnosis::Diagnosis(): m_draw_grids(false) {
}

void Diagnosis::OnStep(Builder* builder_) {
    for (const auto& i : gAPI->observer().GetChatMessages()) {
        if (i.message == "gg") {
            gHistory.warning() << "The game was finished forcibly." << '\n';
            gAPI->debug().EndGame();
            return;
        }

        if (i.message == "grids:dump") {
            auto info = gAPI->observer().GameInfo();
            sc2::PathingGrid(info).Dump("./pathing_grid.txt");
            sc2::PlacementGrid(info).Dump("./placement_grid.txt");
            sc2::HeightMap(info).Dump("./height_map.txt");
        }

        if (i.message == "grids")
            m_draw_grids = !m_draw_grids;
    }

    if (m_draw_grids) {
        auto info = gAPI->observer().GameInfo();
        auto pathing_grid = sc2::PathingGrid(info);
        auto placement_grid = sc2::PlacementGrid(info);
        auto height_map = sc2::HeightMap(info);

        sc2::Point2D camera = gAPI->observer().GetCameraPos();
        int sx = static_cast<int>(camera.x - 12.0f);
        int sy = static_cast<int>(camera.y - 8.0f);

        for (int x = sx; x < sx + 24; ++x) {
            for (int y = sy; y < sy + 20; y++) {
                sc2::Point2DI pos(x, y);

                if (!gAPI->observer().IsPlayableTile(pos))
                    continue;

                sc2::Color color = sc2::Colors::Red;
                if (placement_grid.IsPlacable(pos))
                    color = sc2::Colors::Green;
                else if (pathing_grid.IsPathable(pos))
                    color = sc2::Colors::Yellow;

                sc2::Point3D tile_pos(
                    static_cast<float>(x),
                    static_cast<float>(y),
                    height_map.TerrainHeight(pos));

                gAPI->debug().DrawTile(tile_pos, color);
                gAPI->debug().DrawText(height_map.TerrainHeight(pos), tile_pos, color);
            }
        }
    }

    gAPI->debug().DrawText("Build order:");

    std::list<Order> orders = builder_->GetOrders();
    if (orders.empty()) {
        gAPI->debug().DrawText("Empty");
    } else {
        for (const auto& i : orders)
            gAPI->debug().DrawText(i.name);
    }

    for (const auto& i : gHub->GetExpansions())
        gAPI->debug().DrawSphere(i.town_hall_location, 0.35f);

    gAPI->debug().SendDebug();
}

void Diagnosis::OnGameEnd() {
    gAPI->control().SaveReplay();
    gHistory.info() << "Replay saved" << '\n';
}
