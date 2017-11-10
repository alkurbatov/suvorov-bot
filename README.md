# suvorov-bot
Starcraft 2 Terran bot.

## Build instructions
### Linux and OS X
1. Build SC2 API project.
```bash
$ git clone --recursive https://github.com/Blizzard/s2client-api && cd s2client-api
$ mkdir build && cd build
$ cmake ../
$ make
```

2. Install the SC2 API libraries and headers to your system.
```bash
# Assuming that you are located in the 'build' directory
# after you finished the previous step
$ cd ../

# Install SC2 API headers.
$ sudo mkdir -p /opt/local/include
$ sudo cp -R include/sc2api /opt/local/include
$ sudo cp -R include/sc2utils /opt/local/include
$ sudo cp -R build/generated/s2clientprotocol /opt/local/include

# Install protobuf headers.
$ sudo cp -R contrib/protobuf/src/google /opt/local/include/sc2api

# Install SC2 API libraries.
$ sudo mkdir -p /opt/local/lib/sc2api
$ sudo cp build/bin/libcivetweb.a /opt/local/lib/sc2api
$ sudo cp build/bin/libprotobuf.a /opt/local/lib/sc2api
$ sudo cp build/bin/libsc2api.a /opt/local/lib/sc2api
$ sudo cp build/bin/libsc2lib.a /opt/local/lib/sc2api
$ sudo cp build/bin/libsc2protocol.a /opt/local/lib/sc2api
$ sudo cp build/bin/libsc2utils.a /opt/local/lib/sc2api
```

3. Build the bot
```bash
$ https://github.com/alkurbatov/suvorov-bot.git && cd suvorov-bot

# Create build directory
mkdir build && cd build

# Generate a Makefile.
# Use 'cmake -DCMAKE_BUILD_TYPE=Debug ../' if debuginfo is needed
$ cmake ../

# Build.
$ make

# Launch the bot with the specified absolute path to a SC2 map.
$ ./bin/Suvorov "/Users/alkurbatov/work/tmp/Ladder2017Season3/InterloperLE.SC2Map"
```

### Windows

Not implemented.

## License

Copyright (c) 2017

Licensed under the [MIT license](LICENSE).
