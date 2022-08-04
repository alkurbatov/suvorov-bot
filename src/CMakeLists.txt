# The MIT License (MIT)
#
# Copyright (c) 2017-2022 Alexander Kurbatov

if (BUILD_FOR_LADDER)
    ADD_DEFINITIONS(-DBUILD_FOR_LADDER)
endif ()

# All the source files for the bot.
file(GLOB_RECURSE BOT_SOURCES "*.cpp" "*.h")

configure_file(Version.h.in ${PROJECT_BINARY_DIR}/Version.h)

include_directories(SYSTEM
    ${PROJECT_SOURCE_DIR}/contrib/cpp-sc2/include
    ${PROJECT_SOURCE_DIR}/contrib/cpp-sc2/contrib/protobuf/src
    ${PROJECT_BINARY_DIR}/contrib/cpp-sc2/generated
    ${PROJECT_BINARY_DIR}
    "."
)

link_directories(${PROJECT_BINARY_DIR}/contrib/cpp-sc2/bin)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
elseif (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic \
        -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor \
        -Wold-style-cast -Wconversion -Wsign-conversion -Winit-self \
        -Wunreachable-code"
    )
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# Create the executable.
add_executable(Suvorov ${BOT_SOURCES})
target_link_libraries(Suvorov PRIVATE cpp_sc2)

if (MINGW)
    target_link_libraries(Suvorov PRIVATE ssp)
endif()

if (APPLE)
    target_link_libraries(Suvorov PRIVATE "-framework Carbon")
endif ()

# Linux specific.
if (UNIX AND NOT APPLE)
    target_link_libraries(Suvorov PRIVATE pthread dl)
endif ()
