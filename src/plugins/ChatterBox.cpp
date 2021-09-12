// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#include "ChatterBox.h"
#include "Version.h"
#include "core/API.h"

#include <string>

void ChatterBox::OnGameStart(Builder*) {
    std::string hello(
        std::string(PROJECT_NAME) +
        " v" + PROJECT_VERSION +
        " by @" + PROJECT_AUTHOR);

    gAPI->action().SendMessage(hello);
    gAPI->action().SendMessage("gl hf");
}

void ChatterBox::OnStep(Builder*) {
}
