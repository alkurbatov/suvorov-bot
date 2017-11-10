# All the source files for the bot.
file(GLOB BOT_SOURCES "*.cpp" "*.h")

include_directories(SYSTEM
    ${Boost_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/s2client-api/include
    ${PROJECT_SOURCE_DIR}/s2client-api/contrib/protobuf/src
    ${PROJECT_BINARY_DIR}/s2client-api/generated
)

link_directories(${PROJECT_BINARY_DIR}/s2client-api/bin)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -Weffc++ -Woverloaded-virtual \
-Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Wconversion \
-Wsign-conversion -Winit-self -Wunreachable-code \
")

# Request dynamic linking for Boost_LOG_LIBRARY.
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

# Create the executable.
add_executable(Suvorov ${BOT_SOURCES})
target_link_libraries(Suvorov
    ${Boost_LOG_LIBRARY} ${Boost_LOG_SETUP_LIBRARY}
    ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY}
    sc2api sc2lib sc2utils sc2protocol civetweb protobuf
)

if (APPLE)
    target_link_libraries(Suvorov "-framework Carbon")
endif ()

# Linux specific.
if (UNIX AND NOT APPLE)
    target_link_libraries(Suvorov pthread dl)
endif ()
