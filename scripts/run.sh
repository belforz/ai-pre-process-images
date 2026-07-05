#!/bin/bash

# Executable script for running the main agent of the AI-powered preproccessor project
# When called with no arguments, processes all images in ./images/ with default EXIF orientation.

EXECUTABLE="./build/ai-preproccessor"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable not found at $EXECUTABLE"
    exit 1
fi

if [ "$#" -gt 0 ]; then
    echo "Running $EXECUTABLE $@ ..."
    "$EXECUTABLE" "$@"
else
    # Collect all supported image files from ./images/ (non-recursive, skip temp/)
    SUPPORTED_EXTS="jpg|jpeg|png|tiff|tif|webp|bmp"
    IMAGE_ARGS=()
    while IFS= read -r -d '' img; do
        IMAGE_ARGS+=("$img")
    done < <(find /home/belforz/dataset/vitalidade/candidatas_vitalidade -maxdepth 1 -type f \
        | grep -iE "\.($SUPPORTED_EXTS)$" \
        | sort \
        | tr '\n' '\0')

    if [ "${#IMAGE_ARGS[@]}" -eq 0 ]; then
        echo "No supported images found in /home/belforz/dataset/vitalidade/candidatas_vitalidade"
        exit 1
    fi

    echo "Processing ${#IMAGE_ARGS[@]} images with default EXIF orientation..."
    "$EXECUTABLE" "${IMAGE_ARGS[@]}"
fi