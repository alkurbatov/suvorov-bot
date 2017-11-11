# suvorov-bot

[![Build Status](https://travis-ci.org/alkurbatov/suvorov-bot.png?branch=master)](https://travis-ci.org/alkurbatov/suvorov-bot)

Starcraft 2 Terran bot.

## Build instructions
### Linux and OS X
```bash
# Get the project.
$ git clone --recursive https://github.com/alkurbatov/suvorov-bot.git && cd suvorov-bot

# Create build directory.
$ mkdir build && cd build

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
