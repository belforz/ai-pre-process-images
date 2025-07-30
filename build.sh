#!/bin/bash

echo "🔄 Making dist for directory ..."
mkdir -p build
cd build

echo "⚙️ Running cmake..."
cmake ..

echo "🧱 Compiling cmake..."
make

echo "✅ Build completed!"
