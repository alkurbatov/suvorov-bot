// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "Converter.h"
#include "Dispatcher.h"
#include "Historican.h"

#include <sc2api/sc2_coordinator.h>
#include <sc2api/sc2_gametypes.h>
#include <sc2utils/sc2_arg_parser.h>
#include <sc2utils/sc2_manage_process.h>

#include <iostream>

#ifdef DEBUG
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Provide either name of the map file or path to it!" << std::endl;
        return -1;
    }

    gHistory.Init("history.log");

    sc2::Coordinator coordinator;
    coordinator.LoadSettings(1, argv);

    Dispatcher bot;
    coordinator.SetParticipants({
        CreateParticipant(sc2::Race::Random, &bot),
        CreateComputer(sc2::Race::Random, sc2::Difficulty::CheatInsane)
    });

    coordinator.LaunchStarcraft();
    coordinator.StartGame(argv[1]);

    while (coordinator.Update()) {
        // Slow down game speed for better look & feel while making experiments.
        sc2::SleepFor(15);
    }

    return 0;
}
#else

struct ConnectionOptions {
    int32_t GamePort;
    int32_t StartPort;
    std::string ServerAddress;
    bool ComputerOpponent;
    sc2::Difficulty ComputerDifficulty;
    sc2::Race ComputerRace;
};

void ParseArguments(int argc, char* argv[], ConnectionOptions* options_) {
    sc2::ArgParser arg_parser(argv[0]);
    arg_parser.AddOptions({
            {"-g", "--GamePort", "Port of client to connect to", false},
            {"-o", "--StartPort", "Starting server port", false},
            {"-l", "--LadderServer", "Ladder server address", false},
            {"-c", "--ComputerOpponent", "If we set up a computer opponent", false},
            {"-a", "--ComputerRace", "Race of computer oppent", false},
            {"-d", "--ComputerDifficulty", "Difficulty of computer opponent", false}
        });

    arg_parser.Parse(argc, argv);

    std::string GamePortStr;
    if (arg_parser.Get("GamePort", GamePortStr))
        options_->GamePort = atoi(GamePortStr.c_str());

    std::string StartPortStr;
    if (arg_parser.Get("StartPort", StartPortStr))
        options_->StartPort = atoi(StartPortStr.c_str());

    arg_parser.Get("LadderServer", options_->ServerAddress);

    std::string CompOpp;
    if (arg_parser.Get("ComputerOpponent", CompOpp)) {
        options_->ComputerOpponent = true;
        std::string CompRace;

        if (arg_parser.Get("ComputerRace", CompRace))
            options_->ComputerRace = convert::StringToRace(CompRace);

        std::string CompDiff;
        if (arg_parser.Get("ComputerDifficulty", CompDiff))
            options_->ComputerDifficulty = convert::StringToDifficulty(CompDiff);
    }

    options_->ComputerOpponent = false;
}

int main(int argc, char* argv[]) {
    ConnectionOptions options;
    ParseArguments(argc, argv, &options);

    sc2::Coordinator coordinator;
    if (!coordinator.LoadSettings(argc, argv))
        return -1;

    Dispatcher bot;

    size_t num_agents;
    if (options.ComputerOpponent) {
        num_agents = 1;
        coordinator.SetParticipants({
            CreateParticipant(sc2::Race::Random, &bot),
            CreateComputer(options.ComputerRace, options.ComputerDifficulty)
            });
    } else {
        num_agents = 2;
        coordinator.SetParticipants({CreateParticipant(sc2::Race::Terran, &bot)});
    }

    std::cout << "Connecting to port " << options.GamePort << std::endl;
    coordinator.Connect(options.GamePort);
    coordinator.SetupPorts(num_agents, options.StartPort, false);
    coordinator.JoinGame();
    coordinator.SetTimeoutMS(10000);
    std::cout << " Successfully joined game" << std::endl;

    while (coordinator.Update()) {
    }

    return 0;
}
#endif
