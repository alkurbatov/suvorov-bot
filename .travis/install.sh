#!/bin/bash
#
# The MIT License (MIT)
#
# Copyright (c) 2017-2021 Alexander Kurbatov

set -ev

if [[ "${TRAVIS_OS_NAME}" == "windows" ]]; then
  choco install visualstudio2019community
  choco install visualstudio2019-workload-nativedesktop
fi

curl -o cpplint.py https://raw.githubusercontent.com/google/styleguide/gh-pages/cpplint/cpplint.py
