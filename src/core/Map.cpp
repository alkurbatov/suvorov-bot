// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

#include "Helpers.h"
#include "Historican.h"
#include "Map.h"
#include "core/API.h"

#include "sc2lib/sc2_search.h"
#include <sc2api/sc2_unit_filters.h>

#include <cmath>

namespace {
Historican gHistory("map");

const float PI = 3.1415927f;

size_t CalculateQueries(float radius, float step_size, const sc2::Point2D& center,
    std::vector<sc2::QueryInterface::PlacementQuery>* queries) {
    sc2::Point2D current_grid, previous_grid(
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max());
    size_t valid_queries = 0;

    // Find a buildable location on the circumference of the sphere
    float loc = 0.0f;
    while (loc < 360.0f) {
        sc2::Point2D point(
            (radius * std::cos((loc * PI) / 180.0f)) + center.x,
            (radius * std::sin((loc * PI) / 180.0f)) + center.y);

        sc2::QueryInterface::PlacementQuery query(
            sc2::ABILITY_ID::BUILD_COMMANDCENTER, point);

        current_grid = sc2::Point2D(std::floor(point.x), std::floor(point.y));

        if (previous_grid != current_grid) {
            queries->push_back(query);
            ++valid_queries;
        }

        previous_grid = current_grid;
        loc += step_size;
    }

    return valid_queries;
}

struct Cluster {
    explicit Cluster(uint64_t id_);

    void AddResource(const sc2::Unit* resource_);

    float Height() const;

    sc2::Point3D Center() const;

    uint64_t id;
    sc2::Point3D center_of_mass;
    sc2::Point3D town_hall_location;
    std::vector<sc2::Point3D> points;
    std::vector<const sc2::Unit*> resources;
};

typedef std::vector<Cluster> Clusters;

Cluster::Cluster(uint64_t id_): id(id_) {
    points.reserve(10);
}

void Cluster::AddResource(const sc2::Unit* resource_) {
    resources.push_back(resource_);

    if (points.empty()) {
        center_of_mass = resource_->pos;
    } else {
        center_of_mass =
            (center_of_mass * static_cast<float>(points.size() - 1) + resource_->pos)
                / static_cast<float>(points.size());
    }

    points.push_back(resource_->pos);
}

float Cluster::Height() const {
    if (points.empty())
        return 0.0f;

    return points.back().z;
}

sc2::Point3D Cluster::Center() const {
    return sc2::Point3D(center_of_mass.x, center_of_mass.y, Height());
}

}  // namespace

Expansion::Expansion(const sc2::Point3D& town_hall_location_):
    town_hall_location(town_hall_location_), owner(Owner::NEUTRAL) {
}

void Expansion::AddResource(const sc2::Unit* resource_) {
    if (sc2::IsGeyser()(resource_->unit_type))
        geysers.push_back(resource_->tag);
    else
        minerals.push_back(resource_->tag);
}

const sc2::Unit* Expansion::getClosestMineralTo(const sc2::Point2D& point_) const {
    float distance = std::numeric_limits<float>::max();

    const sc2::Unit* target = nullptr;
    for (const auto i : minerals) {
        const sc2::Unit* unit = gAPI->observer().GetUnit(i);
        if (!unit)
            continue;

        float d = sc2::DistanceSquared2D(unit->pos, point_);
        if (d < distance) {
            distance = d;
            target = unit;
        }
    }

    return target;
}

Expansions CalculateExpansionLocations() {
    Clusters clusters;
    clusters.reserve(20);

    auto resources = gAPI->observer().GetUnits(
        IsFoggyResource(),
        sc2::Unit::Alliance::Neutral);

    if (resources.Empty()) {
        gHistory.warning() << "No expansions locations could be found!" << std::endl;
        return Expansions();
    }

    for (const auto& i : resources()) {
        bool cluster_found = false;

        for (auto& j : clusters) {
            if (sc2::DistanceSquared3D(i->pos, j.points.back()) < 225.0f) {
                j.AddResource(&(*i));
                cluster_found = true;
                break;
            }
        }

        if (!cluster_found) {
            clusters.emplace_back(clusters.size());
            clusters.back().AddResource(&(*i));
        }
    }

    std::vector<size_t> query_size;
    std::vector<sc2::QueryInterface::PlacementQuery> queries;
    for (auto& i : clusters)
        query_size.push_back(CalculateQueries(5.3f, 0.5f, i.Center(), &queries));

    std::vector<bool> results = gAPI->query().CanBePlaced(queries);

    size_t start_index = 0;
    Expansions expansions;
    for (auto& i : clusters) {
        for (size_t j = start_index, e = start_index + query_size[i.id]; j < e; ++j) {
            if (!results[j])
                continue;

            sc2::Point3D town_hall_location = sc2::Point3D(
                queries[j].target_pos.x,
                queries[j].target_pos.y,
                i.Height());
            expansions.emplace_back(town_hall_location);
            for (auto res: i.resources)
                expansions.back().AddResource(res);
            break;
        }

        start_index += query_size[i.id];
    }

    // NOTE (implusecloud): Include start location, since CanBePlaced will fail
    // TownHall tag will be added during its OnCreated event.
    expansions.emplace_back(gAPI->observer().StartingLocation());
    
    auto minerals = gAPI->observer().GetUnits(
        sc2::IsVisibleMineralPatch(), sc2::Unit::Alliance::Neutral);
    for (auto& i : minerals())
        expansions.back().AddResource(i);
    
    auto geysers = gAPI->observer().GetUnits(
        sc2::IsVisibleGeyser(), sc2::Unit::Alliance::Neutral);
    for (auto& i : geysers())
        expansions.back().AddResource(i);

    return expansions;
}
