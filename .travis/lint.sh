#!/bin/bash
#
# The MIT License (MIT)
#
# Copyright (c) 2017-2018 Alexander Kurbatov

python cpplint.py \
    --linelength=90 \
    --filter=-build/header_guard,-build/include,-whitespace/indent \
    src/*
