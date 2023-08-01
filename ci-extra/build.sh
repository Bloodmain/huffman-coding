#!/bin/bash
set -euo pipefail
IFS=$' \t\n'

mkdir -p cmake-build-$1
rm -rf cmake-build-$1/*
cmake "-DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake" -GNinja --preset $1 \
  -DTREAT_WARNINGS_AS_ERRORS=ON -S .
cmake --build cmake-build-$1
