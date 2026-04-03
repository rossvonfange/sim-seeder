#!/bin/bash
# Clean environment wrapper for seeder-simulator
# Avoids snap glibc conflicts while preserving necessary environment

exec env -i \
    PATH="$PATH" \
    HOME="$HOME" \
    USER="$USER" \
    DISPLAY="${DISPLAY:-:99}" \
    XDG_RUNTIME_DIR="${XDG_RUNTIME_DIR:-/tmp/runtime-$USER}" \
    /home/cycix/sim-seeder/build/seeder-simulator "$@"
