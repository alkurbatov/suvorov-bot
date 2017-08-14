# All the source files for the bot.
file(GLOB BOT_SOURCES "*.cpp" "*.h")

include_directories(SYSTEM "${SC2Api_INCLUDE_DIRS}")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -Weffc++ -Woverloaded-virtual \
-Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Wconversion \
-Wsign-conversion -Winit-self -Wunreachable-code \
")

# Create the executable.
add_executable(Suvorov ${BOT_SOURCES})
target_link_libraries(Suvorov ${SC2Api_LIBRARIES})

if (APPLE)
    target_link_libraries(Suvorov "-framework Carbon")
endif ()
