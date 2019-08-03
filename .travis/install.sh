#!/bin/bash
#
# The MIT License (MIT)
#
# Copyright (c) 2017-2019 Alexander Kurbatov

if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
    sudo apt-get install -y g++-5
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 90
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-5 90
fi

curl -o cpplint.py https://raw.githubusercontent.com/google/styleguide/gh-pages/cpplint/cpplint.py
