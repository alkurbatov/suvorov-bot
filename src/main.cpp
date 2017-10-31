#include <sc2api/sc2_gametypes.h>
#include <sc2api/sc2_coordinator.h>
#include <sc2utils/sc2_manage_process.h>

#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "Dispatcher.h"

void initLogging()
{
    namespace logging = boost::log;
    namespace expr = boost::log::expressions;
    namespace keywords = boost::log::keywords;

    // Log message example:
    // HH:MI:SS: <error> An error severity message
    logging::add_file_log(
        keywords::file_name = "game_%N.log",

        keywords::format =
            (expr::stream
             << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S")
             << ": <" << logging::trivial::severity
             << "> " << expr::smessage)
    );

    logging::add_common_attributes();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Please specify absolute path to the map file!" << std::endl;
        return -1;
    }

    initLogging();

    sc2::Coordinator coordinator;
    coordinator.LoadSettings(1, argv);

    Dispatcher bot;
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
