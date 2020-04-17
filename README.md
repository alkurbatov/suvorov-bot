# suvorov-bot

[![Build Status](https://travis-ci.org/alkurbatov/suvorov-bot.svg?branch=master)](https://travis-ci.org/alkurbatov/suvorov-bot)

- [suvorov-bot](#suvorov-bot)
    - [About](#about)
    - [Support](#support)
    - [Build requirements](#build-requirements)
    - [Build instructions](#build-instructions)
    - [Coding Standard](#coding-standard)
    - [License](#license)

## About
Starcraft 2 rule-based bot capable to play for all races.

Suvorov currently provides the following features:
* Supports Windows, OS X and Linux.
* Plays all 3 races.
* Manages resource gathering and worker allocation.
* Supports all available buildings upgrades (e.g. Orbital Command, Planetary Fortress, Lair).
* Supports all available zerg units mutations (e.g. Overseer, Baneling, Ravager).
* Calls down MULEs, casts chronoboost.
* Builds Terran addons.
* Warps in gate units.
* Automatically builds new supplies when needed.
* Supports launch under [Sc2LadderServer](https://github.com/Cryptyc/Sc2LadderServer).

## Notable forks
* [MulleMech](https://github.com/ludlyl/MulleMech) - a terran bot by @ludlyl

## Support
Feel free to post questions on the unofficial Starcraft II AI Discord server. [Invite Link](https://discordapp.com/invite/Emm5Ztz)

## Build requirements
1. Download (the password is iagreetotheeula) at least one of the following map packs:
  * [Ladder 2019 Season 1](http://blzdistsc2-a.akamaihd.net/MapPacks/Ladder2019Season1.zip)
  * [Ladder 2019 Season 2](http://blzdistsc2-a.akamaihd.net/MapPacks/Ladder2019Season2.zip)
  * [Ladder 2019 Season 3](http://blzdistsc2-a.akamaihd.net/MapPacks/Ladder2019Season3.zip)

2. Put the downloaded maps into the Maps folder (create it if the folder doesn't exist):
  * Windows: C:\Program Files\StarCraft II\Maps
  * OS X: /Applications/StarCraft II/Maps
  * Linux: anywhere.

3. Download and install [CMake](https://cmake.org/download/).

4. A compiler with C++14 support.

5. Windows: Download and install Visual Studio ([2019](https://www.visualstudio.com/downloads/) or older).

6. Linux: Install 'gcc-c++'.

7. Linux: Install the 'make' utility.

8. OS X: Install XCode.

9. OS X: Install XCode command-line tools.

## Build instructions

### Windows (Visual Studio)
```bat
:: Get the project.
$ git clone --recursive https://github.com/alkurbatov/suvorov-bot.git
$ cd suvorov-bot

:: Create build directory.
$ mkdir build
$ cd build

:: Create Visual Studio project files.
:: For Visual Studio 2019.
$ cmake ../ -G "Visual Studio 16 2019"
:: For Visual Studio 2017.
$ cmake ../ -G "Visual Studio 15 2017 Win64"

:: Build the project using Visual Studio.
$ start Suvorov.sln

:: Launch the bot with the specified path to a SC2 map, e.g.
$ bin\Debug\Suvorov.exe Ladder2019Season3/AcropolisLE.SC2Map
```

### Windows (nmake)
```bat
:: Get the project.
$ git clone --recursive https://github.com/alkurbatov/suvorov-bot.git
$ cd suvorov-bot

:: Create build directory.
$ mkdir build
$ cd build

:: Create makefiles.
$ cmake ../ -G "NMake Makefiles"

:: Build the project with nmake.
$ nmake
```

### Linux (make)
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
$ ./bin/Suvorov "/Users/alkurbatov/work/tmp/Ladder2019Season3/AcropolisLE.SC2Map"
```

### Linux (gmake)
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
$ gmake

# Launch the bot with the specified absolute path to a SC2 map, e.g.
$ ./bin/Suvorov "/Users/alkurbatov/work/tmp/Ladder2019Season3/AcropolisLE.SC2Map"
```

### OS X (Xcode)
```bash
# Get the project.
$ git clone --recursive https://github.com/alkurbatov/suvorov-bot.git && cd suvorov-bot

# Create build directory.
$ mkdir build && cd build

# Generate a Makefile.
# Use 'cmake -DCMAKE_BUILD_TYPE=Debug ../' if debuginfo is needed
# Debug build also contains additional debug features and chat commands support.
$ cmake ../ -G Xcode

# Build the project using Xcode.
$ open Suvorov.xcodeproj/

# Launch the bot with the specified path to a SC2 map, e.g.
$ ./bin/Suvorov "Ladder2019Season3/AcropolisLE.SC2Map"
```

### OS X (make)
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
$ ./bin/Suvorov "Ladder2019Season3/AcropolisLE.SC2Map"
```

## Coding Standard
We follow slightly modified [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
See [.travis/lint.sh](.travis/lint.sh) for details.

## License
Copyright (c) 2017-2020 Alexander Kurbatov

Licensed under the [MIT license](LICENSE).
