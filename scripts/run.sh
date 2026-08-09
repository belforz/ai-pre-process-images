#!/bin/bash

# Executable script for running the main agent of the AI-powered preproccessor project.
#
# Usage:
#   ./scripts/run.sh                                   # process default local dataset folder
#   ./scripts/run.sh <payload-json-or-file> ...         # AI trigger mode (passthrough)
#   ./scripts/run.sh --dir <photos_dir> [--category <name>] [exif]
#       Scans <photos_dir> (non-recursive) for supported images and processes them.
#       <photos_dir> is typically the dynamic uploads folder created per upload:
#         /home/belforz/photus-system/data/uploads/{uuid}_{YYYYMMDD_HHMMSS}/nome_da_foto.formato
#       --category tags every image found with <name> in local/images_index.json.

EXECUTABLE="./build/ai-preproccessor"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable not found at $EXECUTABLE"
    exit 1
fi

SUPPORTED_EXTS="jpg|jpeg|png|tiff|tif|webp|bmp"

PHOTOS_DIR=""
CATEGORY=""
PASSTHROUGH_ARGS=()

while [ "$#" -gt 0 ]; do
    case "$1" in
        --dir)
            PHOTOS_DIR="$2"
            shift 2
            ;;
        --category)
            CATEGORY="$2"
            shift 2
            ;;
        *)
            PASSTHROUGH_ARGS+=("$1")
            shift
            ;;
    esac
done

IMAGE_ARGS=()

if [ -n "$PHOTOS_DIR" ]; then
    if [ ! -d "$PHOTOS_DIR" ]; then
        echo "Photos directory not found: $PHOTOS_DIR"
        exit 1
    fi

    while IFS= read -r -d '' img; do
        IMAGE_ARGS+=("$img")
    done < <(find "$PHOTOS_DIR" -maxdepth 1 -type f \
        | grep -iE "\.($SUPPORTED_EXTS)$" \
        | sort \
        | tr '\n' '\0')

    if [ "${#IMAGE_ARGS[@]}" -eq 0 ]; then
        echo "No supported images found in $PHOTOS_DIR"
        exit 1
    fi

    CMD_ARGS=()
    if [ -n "$CATEGORY" ]; then
        CMD_ARGS+=(--category "$CATEGORY")
    fi
    CMD_ARGS+=("${IMAGE_ARGS[@]}" "${PASSTHROUGH_ARGS[@]}")

    echo "Processing ${#IMAGE_ARGS[@]} images from $PHOTOS_DIR (category: ${CATEGORY:-none})..."
    "$EXECUTABLE" "${CMD_ARGS[@]}"
elif [ "${#PASSTHROUGH_ARGS[@]}" -gt 0 ]; then
    echo "Running $EXECUTABLE ${PASSTHROUGH_ARGS[@]} ..."
    "$EXECUTABLE" "${PASSTHROUGH_ARGS[@]}"
else
    # Fallback for local/dev testing: process the default local dataset folder.
    while IFS= read -r -d '' img; do
        IMAGE_ARGS+=("$img")
    done < <(find /home/belforz/dataset/noturno/candidatas_noturno -maxdepth 1 -type f \
        | grep -iE "\.($SUPPORTED_EXTS)$" \
        | sort \
        | tr '\n' '\0')

    if [ "${#IMAGE_ARGS[@]}" -eq 0 ]; then
        echo "No supported images found in /home/belforz/dataset/noturno/candidatas_noturno"
        exit 1
    fi

    echo "Processing ${#IMAGE_ARGS[@]} images with default EXIF orientation..."
    "$EXECUTABLE" "${IMAGE_ARGS[@]}"
fi
