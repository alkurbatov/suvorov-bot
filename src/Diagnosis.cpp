// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "Diagnosis.h"

void Diagnosis::OnStep() const {
    gAPI->debug().SendDebug();
}

void Diagnosis::ShowBuildOrder(const std::list<Order>& orders_) const {
    gAPI->debug().DrawText("Build order:");

    if (orders_.empty()) {
        gAPI->debug().DrawText("Empty");
        return;
    }

    for (const auto& i : orders_)
        gAPI->debug().DrawText(i.data.name);
}
