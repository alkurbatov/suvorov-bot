// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Alexander Kurbatov

#include "Dispatcher.h"
#include "Historican.h"
#include "core/Converter.h"

#include <sc2api/sc2_coordinator.h>
#include <sc2api/sc2_gametypes.h>
#include <sc2utils/sc2_arg_parser.h>
#include <sc2utils/sc2_manage_process.h>

#include <iostream>
#include <string>

#ifdef BUILD_FOR_LADDER

namespace {
struct Options {
    Options(): GamePort(0), StartPort(0) {
    }

    int32_t GamePort;
    int32_t StartPort;
    std::string ServerAddress;
    std::string OpponentId;
};

void ParseArguments(int argc, char* argv[], Options* options_) {
    sc2::ArgParser arg_parser(argv[0]);
    arg_parser.AddOptions(
        {
            {"-g", "--GamePort", "Port of client to connect to", false},
            {"-o", "--StartPort", "Starting server port", false},
            {"-l", "--LadderServer", "Ladder server address", false},
            {"-x", "--OpponentId", "PlayerId of opponent", false},
        });

    arg_parser.Parse(argc, argv);

    std::string GamePortStr;
    if (arg_parser.Get("GamePort", GamePortStr))
        options_->GamePort = atoi(GamePortStr.c_str());

    std::string StartPortStr;
    if (arg_parser.Get("StartPort", StartPortStr))
        options_->StartPort = atoi(StartPortStr.c_str());

    std::string OpponentId;
    if (arg_parser.Get("OpponentId", OpponentId))
        options_->OpponentId = OpponentId;

    arg_parser.Get("LadderServer", options_->ServerAddress);
}

}  // namespace

int main(int argc, char* argv[]) {
    Options options;
    ParseArguments(argc, argv, &options);

    sc2::Coordinator coordinator;
    Dispatcher bot(options.OpponentId);

    const size_t num_agents = 2;
    coordinator.SetParticipants({
        CreateParticipant(sc2::Race::Random, &bot, "Suvorov")
    });

    std::cout << "Connecting to port " << options.GamePort << std::endl;
    coordinator.Connect(options.GamePort);
    coordinator.SetupPorts(num_agents, options.StartPort, false);

    // NB (alkurbatov): Increase speed of steps processing.
    // Disables ability to control your bot during game.
    // Recommended for competitions.
    coordinator.SetRawAffectsSelection(true);

    coordinator.JoinGame();
    coordinator.SetTimeoutMS(10000);
    std::cout << "Successfully joined game" << std::endl;

    while (coordinator.Update()) {
    }

    return 0;
}

#else

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Provide either name of the map file or path to it!" << std::endl;
        return -1;
    }

    Historican::Init("history.log");

    sc2::Coordinator coordinator;
    coordinator.LoadSettings(argc, argv);

    // NOTE (alkurbatov): Uncomment to start the game in full screen mode.
    // coordinator.SetFullScreen(true);

    // NOTE (alkurbatov): Uncomment to play at normal speed.
    // coordinator.SetRealtime(true);

    Dispatcher bot("TrainingDummy");
    coordinator.SetParticipants({
        CreateParticipant(sc2::Race::Random, &bot, "Suvorov"),
        CreateComputer(
            sc2::Race::Random,
            sc2::Difficulty::CheatInsane,
            sc2::AIBuild::Rush,
            "CheatInsane"
        )
    });

    coordinator.LaunchStarcraft();
    coordinator.StartGame(argv[1]);

    while (coordinator.Update()) {
    }

    return 0;
}
#endif
