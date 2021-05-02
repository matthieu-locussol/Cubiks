#!/bin/sh bash

mkdir -p build
cd build

cmake .. -DCMAKE_BUILD_TYPE=Debug -DCUBIKS_TESTS=ON
cmake --build . --config "Debug" --parallel 4
