#include <sc2api/sc2_gametypes.h>
#include <sc2api/sc2_coordinator.h>
#include <sc2utils/sc2_manage_process.h>

#include "Bot.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Please specify absolute path to the map file!" << std::endl;
        return -1;
    }

    sc2::Coordinator coordinator;
    coordinator.LoadSettings(1, argv);

    Bot bot;
    coordinator.SetParticipants({
        CreateParticipant(sc2::Race::Terran, &bot),
        CreateComputer(sc2::Race::Zerg)
    });

    coordinator.LaunchStarcraft();
    coordinator.StartGame(argv[1]);

    while (coordinator.Update()) {
        // Slow down game speed for better look & feel while making experiments.
        sc2::SleepFor(15);
    }

    return 0;
}
