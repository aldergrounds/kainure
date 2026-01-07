#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
PROJECT_ROOT="$(readlink -f "$SCRIPT_DIR/../../")"
OUTPUT_DIR="$PROJECT_ROOT/compiled/Linux"
BUILD_CACHE_DIR="$PROJECT_ROOT/compiled/Linux/build_temp"
IMAGE_NAME="kainure-builder-x86"

mkdir -p "$OUTPUT_DIR"

if [[ "$(docker images -q $IMAGE_NAME 2> /dev/null)" == "" ]] || [[ "$1" == "rebuild" ]]; then
    echo "Building Docker image..."
    docker build -t $IMAGE_NAME "$SCRIPT_DIR"
else
    echo "Using cached build image..."
fi

echo "Compiling..."

docker run --rm \
    -u $(id -u):$(id -g) \
    -v /etc/passwd:/etc/passwd:ro \
    -v /etc/group:/etc/group:ro \
    -v "$PROJECT_ROOT":/workspace \
    -w /workspace \
    -e HOME=/tmp \
    kainure-builder-x86 \
    /bin/bash -c "
        rm -rf compiled/Linux/build_temp && \
        cmake -S src -B compiled/Linux/build_temp -DCMAKE_BUILD_TYPE=Release -Wno-dev && \
        cmake --build compiled/Linux/build_temp -j\$(nproc) && \
        rm -rf compiled/Linux/build_temp
    "

if [ -f "$OUTPUT_DIR/Kainure.so" ]; then
    echo "Completed: compiled/Linux/Kainure.so"
else
    echo "Error: Build failed - Kainure.so not found"
    exit 1
fi
