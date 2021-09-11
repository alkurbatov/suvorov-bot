// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#include "ChatterBox.h"
#include "Version.h"
#include "core/API.h"

#include <string>

void ChatterBox::OnGameStart(Builder*) {
    std::string hello(std::string("Suvorov v") + PROJECT_VERSION_STR + " by @alkurbatov");
    gAPI->action().SendMessage(hello);
    gAPI->action().SendMessage("gl hf");
}

void ChatterBox::OnStep(Builder*) {
}
