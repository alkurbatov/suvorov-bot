// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "GateUnit.h"
#include "Unit.h"
#include "core/API.h"
#include "core/Helpers.h"

bool GateUnit::Build(Order* order_) {
    auto warp_gates = gAPI->observer().GetUnits(
        IsIdleUnit(sc2::UNIT_TYPEID::PROTOSS_WARPGATE));
    if (warp_gates().empty())
        return Unit(sc2::UNIT_TYPEID::PROTOSS_GATEWAY).Build(order_);

    auto pylons = gAPI->observer().GetUnits(
        IsUnit(sc2::UNIT_TYPEID::PROTOSS_PYLON));
    if (pylons().empty())
        return false;

    // FIXME (alkurbatov): Looks like there is no way to determine
    // how many changes a warpgate has now. This should be fixed.
    // Requesting random unit is just a way to mitigate this problem.
    const sc2::Unit* warp_gate = warp_gates.GetRandomUnit();

    const sc2::Unit* pylon = pylons.GetClosestUnit(warp_gate->tag);
    if (!pylon)
        return false;

    order_->assignee = warp_gate->tag;

    // NOTE (alkurbatov): A bit of numbers magic here.
    // We assume that Starcraft2 protol IDs are stable, thus we can safely
    // convert e.g. TRAIN_ZEALOT command to TRAINWARP_ZEALOT
    // using delta value similar for all operations with the gates.
    order_->ability_id = order_->ability_id + 497;

    sc2::Point2D point;
    unsigned attempt = 0;
    do {
        point.x = pylon->pos.x + static_cast<float>(sc2::GetRandomInteger(-5, 5));
        point.y = pylon->pos.y + static_cast<float>(sc2::GetRandomInteger(-5, 5));

        if (++attempt > 30)
            return false;
    } while (!gAPI->query().CanBePlaced(*order_, point));

    gAPI->action().Build(*order_, point);

    return true;
}
