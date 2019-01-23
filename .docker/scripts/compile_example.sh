#!/bin/bash

cd /root/workspace/partons-example/build
cmake -G"Unix Makefiles" ../ -DCMAKE_BUILD_TYPE=Debug
make
