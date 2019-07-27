# suvorov-bot

[![Build Status](https://travis-ci.org/alkurbatov/suvorov-bot.svg?branch=master)](https://travis-ci.org/alkurbatov/suvorov-bot)

- [suvorov-bot](#suvorov-bot)
    - [About](#about)
    - [Support](#support)
    - [Build requirements](#build-requirements)
        - [Windows](#windows)
        - [Linux](#linux)
        - [OS X](#os-x)
    - [Build instructions](#build-instructions)
        - [Windows](#windows)
        - [Linux](#linux)
        - [OS X](#os-x)
    - [Coding Standard](#coding-standard)
    - [License](#license)

## About
Starcraft 2 bot.

Suvorov currently provides the following features:
* Supports Windows, OS X and Linux.
* Plays all 3 races.
* Manages resource gathering and worker allocation.
* Supports all available buildings upgrades (e.g. Orbital Command, Planetary Fortress, Lair).
* Supports all available zerg units mutations (e.g. Overseer, Baneling, Ravager).
* Calls down MULEs, casts chronoboost.
* Warps in gate units.
* Automatically builds new supplies when needed.
* Supports launch under [Sc2LadderServer](https://github.com/Cryptyc/Sc2LadderServer).

## Support
Feel free to post questions on the unofficial Starcraft II AI Discord server. [Invite Link](https://discordapp.com/invite/Emm5Ztz)

## Build requirements
* Download (the password is iagreetotheeula) at least one of the following map packs:
  * [Ladder 2017 Season 1](http://blzdistsc2-a.akamaihd.net/MapPacks/Ladder2017Season1.zip)
  * [Ladder 2017 Season 2](http://blzdistsc2-a.akamaihd.net/MapPacks/Ladder2017Season2.zip)
  * [Ladder 2017 Season 3](http://blzdistsc2-a.akamaihd.net/MapPacks/Ladder2017Season3_Updated.zip)
  * [Ladder 2017 Season 4](http://blzdistsc2-a.akamaihd.net/MapPacks/Ladder2017Season4.zip)
  * [Ladder 2018 Season 1](http://blzdistsc2-a.akamaihd.net/MapPacks/Ladder2018Season1.zip)
  * [Ladder 2018 Season 2](http://blzdistsc2-a.akamaihd.net/MapPacks/Ladder2018Season2_Updated.zip)
* Put the downloaded maps into the Maps folder (create it if the folder doesn't exist):
  * Windows: C:\Program Files\StarCraft II\Maps
  * OS X: /Applications/StarCraft II/Maps
  * Linux: anywhere.
* Download and install [CMake](https://cmake.org/download/).
* A compiler with C++14 support.

### Windows
* Download and install [Visual Studio 2017](https://www.visualstudio.com/downloads/)

### Linux
* Install 'gcc-c++'.
* Install the 'make' utility.

### OS X
* Install XCode command-line tools.

## Build instructions

### Windows
```bat
:: Get the project.
$ git clone --recursive https://github.com/alkurbatov/suvorov-bot.git
$ cd suvorov-bot

:: Disable /WX compiler flag if you want to build the project
:: under fresh compiler/new Visual Studio.
$ git apply hacks/disable_WX.patch

:: Create build directory.
$ mkdir build
$ cd build

:: Generate VS solution.
$ cmake ../ -G "Visual Studio 15 2017 Win64"

:: Build the project using Visual Studio.
$ start Suvorov.sln

:: Launch the bot with the specified path to a SC2 map, e.g.
$ bin\Debug\Suvorov.exe Ladder2017Season3\InterloperLE.SC2Map
```

### Linux
```bash
# Get the project.
$ git clone --recursive https://github.com/alkurbatov/suvorov-bot.git && cd suvorov-bot

# Create build directory.
$ mkdir build && cd build

# Generate a Makefile.
# Use 'cmake -DCMAKE_BUILD_TYPE=Debug ../' if debuginfo is needed
# Debug build also contains additional debug features and chat commands support.
$ cmake ../

# Build.
$ make

# Launch the bot with the specified absolute path to a SC2 map, e.g.
$ ./bin/Suvorov "/Users/alkurbatov/work/tmp/Ladder2017Season3/InterloperLE.SC2Map"
```

### OS X
```bash
# Get the project.
$ git clone --recursive https://github.com/alkurbatov/suvorov-bot.git && cd suvorov-bot

# Create build directory.
$ mkdir build && cd build

# Generate a Makefile.
# Use 'cmake -DCMAKE_BUILD_TYPE=Debug ../' if debuginfo is needed
# Debug build also contains additional debug features and chat commands support.
$ cmake ../

# Build.
$ make

# Launch the bot with the specified path to a SC2 map, e.g.
$ ./bin/Suvorov "Ladder2017Season3/InterloperLE.SC2Map"
```

## Coding Standard
We follow slightly modified [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
See [.travis/lint.sh](.travis/lint.sh) for details.

## License
Copyright (c) 2017-2019 Alexander Kurbatov

Licensed under the [MIT license](LICENSE).
