#!/usr/bin/env bash

set -e

pushd $(dirname $0)
rm -rf build
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make VERBOSE=1
popd
