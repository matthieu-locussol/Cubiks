#!/bin/sh

echo "Running Cubiks..."

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"$PWD/libraries"

chmod +x ./binaries/Cubiks

./binaries/Cubiks 2> /dev/null