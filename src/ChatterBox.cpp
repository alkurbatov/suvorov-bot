// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "ChatterBox.h"
#include "Historican.h"

void ChatterBox::OnGameStart() const {
    gAPI->action().SendMessage("gl hf");
}

void ChatterBox::OnStep() const {
    auto messages = gAPI->observer().GetChatMessages();

    auto it = std::find_if(messages.begin(), messages.end(),
        [](const sc2::ChatMessage& chatMessage_) {
            return chatMessage_.message == "gg";
        });

    if (it == messages.end())
        return;

    gHistory << "The game was finished forcibly." << std::endl;
    gAPI->debug().EndGame();
}
