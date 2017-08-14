#include <sc2api/sc2_gametypes.h>
#include <sc2api/sc2_coordinator.h>

#include "Bot.h"

int main(int argc, char *argv[]) {
    sc2::Coordinator coordinator;
    coordinator.LoadSettings(argc, argv);

    Bot bot;
    coordinator.SetParticipants({
        CreateParticipant(sc2::Race::Terran, &bot),
        CreateComputer(sc2::Race::Zerg)
    });

    coordinator.LaunchStarcraft();
    coordinator.StartGame("/Users/alkurbatov/work/tmp/Ladder2017Season3/InterloperLE.SC2Map");

    while (coordinator.Update()) {
    }

    return 0;
}
