// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "../API.h"
#include "../Historican.h"
#include "ChatterBox.h"

void ChatterBox::OnGameStart() {
    gAPI->action().SendMessage("gl hf");
}

void ChatterBox::OnStep() {
    auto messages = gAPI->observer().GetChatMessages();

    auto it = std::find_if(messages.begin(), messages.end(),
        [](const sc2::ChatMessage& chatMessage_) {
            return chatMessage_.message == "gg";
        });

    if (it == messages.end())
        return;

    gHistory << "[WARNING] The game was finished forcibly." << std::endl;
    gAPI->debug().EndGame();
}
