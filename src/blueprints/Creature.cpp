// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../Hub.h"
#include "Creature.h"
#include "core/API.h"
#include "core/Helpers.h"

bool Creature::Build(Order* order_) {
    return gHub->AssignLarva(order_);
}
