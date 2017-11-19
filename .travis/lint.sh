#!/bin/bash

python cpplint.py \
    --linelength=90 \
    --filter=-build/header_guard,-legal/copyright,-build/include,-whitespace/indent \
    src/*
