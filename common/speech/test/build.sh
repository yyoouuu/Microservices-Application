#!/bin/bash
set -e

SRC=test.cpp
OUT=test

g++ -o0 \
    -g \
    -std=c++11 $SRC \
    -I../speech_party \
    -I../include \
    -I/usr/include/jsoncpp \
    -lcurl -ljsoncpp -lcrypto \
    -o $OUT

echo "Build success: $OUT"
