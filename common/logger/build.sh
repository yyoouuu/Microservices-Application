#!/bin/bash
set -e

# -----------------------
# 配置路径
# -----------------------
PROJECT_ROOT="/home/amoureux/amoureux/Microservices"
LOGGER_SRC="$PROJECT_ROOT/common/logger/src"
LOGGER_INCLUDE="$PROJECT_ROOT/common/logger/include"
SPEECH_INCLUDE="$PROJECT_ROOT/common/speech/include"
SPDLOG_INCLUDE="$PROJECT_ROOT/third_party/spdlog/include"
TEST_DIR="$PROJECT_ROOT/common/logger/test"
OUTPUT_DIR="$TEST_DIR"

# -----------------------
# 编译 libspdlog 静态库：将spdlog库中用到的cpp文件和logger中的cpp文件编译成.o文件，把所有 *.o 对象文件打包成一个静态库，libamo_log.a
# -----------------------
echo "Building libspdlog..."
g++ -g -O0 -std=c++11 -I$LOGGER_INCLUDE -I$SPDLOG_INCLUDE -c $LOGGER_SRC/*.cpp
ar rcs libamo_log.a *.o
rm *.o
echo "libspdlog.a built successfully."


# -----------------------
# 编译 test 可执行文件
# -----------------------
echo "Building test executable..."
g++ -g -O0 -std=c++17 -I$LOGGER_INCLUDE -I$SPEECH_INCLUDE -I$SPDLOG_INCLUDE $TEST_DIR/*.cpp \
    -L. -lamo_log -lpthread -o $OUTPUT_DIR/test

echo "Build success: $OUTPUT_DIR/test"
