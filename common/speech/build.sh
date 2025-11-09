#!/bin/bash
set -e

# -----------------------
# 配置路径
# -----------------------
project_root="/home/amoureux/amoureux/Microservices"

# 头文件路径
logger_src="$project_root/common/logger"
logger_include="$project_root/common/logger"
speech_include="$project_root/strid_party/speech_recognition"
speecher_include="$project_root/common/speech/include"
spdlog_include="$project_root/third_party/spdlog/include"

# 测试目录
test_dir="$project_root/common/speech/test"

# 输出路径
output_dir="$test_dir"

# -----------------------
# 编译 test 可执行文件：speecher用到了日志模块，链接日志模块的库amo_log.a进行编译
# -----------------------
echo "Building test executable..."
g++ -g -O0 -std=c++11 \
    -I$logger_include \
    -I$speech_include \
    -I$spdlog_include \
    -I$speecher_include \
    -I/usr/include/jsoncpp \
    $test_dir/*.cpp \
    -L$logger_include -lamo_log \
    -L. -lcurl -ljsoncpp -lcrypto -pthread \
    -o $output_dir/test

echo "Build success: $output_dir/test"


