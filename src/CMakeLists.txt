# All the source files for the bot.
file(GLOB BOT_SOURCES "*.cpp" "*.h")

include_directories("${SC2Api_INCLUDE_DIRS}")

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

if (APPLE)
    # Supress annoying 'deprecated' warnings caused by sc2api headers.
    set(CMAKE_CXX_FLAGS "-Wno-deprecated")
endif ()

# Create the executable.
add_executable(Suvorov ${BOT_SOURCES})
target_link_libraries(Suvorov ${SC2Api_LIBRARIES})

if (APPLE)
    target_link_libraries(Suvorov "-framework Carbon")
endif ()
