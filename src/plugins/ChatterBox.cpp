// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "Historican.h"
#include "ChatterBox.h"
#include "core/API.h"

void ChatterBox::OnGameStart(Builder*) {
    gAPI->action().SendMessage("Suvorov v0.0.2 by @alkurbatov");
    gAPI->action().SendMessage("gl hf");
}

void ChatterBox::OnStep(Builder*) {
#ifdef DEBUG
    auto messages = gAPI->observer().GetChatMessages();

    auto it = std::find_if(messages.begin(), messages.end(),
        [](const sc2::ChatMessage& chatMessage_) {
            return chatMessage_.message == "gg";
        });

    if (it == messages.end())
        return;

    gHistory.warning() << "The game was finished forcibly." << std::endl;
    gAPI->debug().EndGame();
#endif
}
