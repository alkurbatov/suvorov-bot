// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#include "ChatterBox.h"
#include "core/API.h"

void ChatterBox::OnGameStart(Builder*) {
    gAPI->action().SendMessage("Suvorov v0.0.4 by @alkurbatov");
    gAPI->action().SendMessage("gl hf");
}

void ChatterBox::OnStep(Builder*) {
}
