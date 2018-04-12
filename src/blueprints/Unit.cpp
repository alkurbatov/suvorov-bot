// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "Unit.h"

Unit::Unit(): Blueprint(false) {
}


bool Unit::Build(Order* order_) {
    if (!order_->assignee)
        return false;

    gAPI->action().Command(*order_);

    return true;
}
