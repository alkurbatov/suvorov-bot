#!/bin/bash
#
# The MIT License (MIT)
#
# Copyright (c) 2017-2019 Alexander Kurbatov

# On Windows, configures path to VS2019 to give cmake a hint where to search it.

set -ev

echo "Configuring for ${BUILD_CONFIGURATION}"

if [[ "$TRAVIS_OS_NAME" == "windows" ]]; then
  export VS160COMNTOOLS="/c/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/Tools"
fi
