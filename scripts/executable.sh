#!/bin/bash

# Executable script for running the main agent of the AI-powered curation technician project

EXECUTABLE="./build/ai-preproccessor"

if [ -f "$EXECUTABLE" ]; then
    echo " Running $EXECUTABLE $@ ..."
    "$EXECUTABLE" "$@"
else
    echo " Executable not found at $EXECUTABLE"
    exit 1
fi