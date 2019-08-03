#!/bin/bash
#
# The MIT License (MIT)
#
# Copyright (c) 2017-2019 Alexander Kurbatov

python cpplint.py \
    --linelength=90 \
    --filter=-build/header_guard,-build/include,-build/c++11 \
    src/* src/blueprints/* src/core/* src/objects/* src/plugins/* \
    src/strategies/* src/strategies/protoss/* src/strategies/terran/* \
    src/strategies/zerg/*
