#!/bin/bash
set -e

# -----------------------
# 配置路径
# -----------------------
#二次封装头文件
PROJECT_ROOT="/home/amoureux/amoureux/Microservices"
LOGGER_SRC="$PROJECT_ROOT/common/logger/src"
LOGGER_INCLUDE="$PROJECT_ROOT/common/logger/include"
AMO_ETCD_INCLUDE="$PROJECT_ROOT/common/amo_etcd/include"
AMO_ETCD_SRC="$PROJECT_ROOT/common/amo_etcd/src"
#二次封装库
LOGGER_LIB="$PROJECT_ROOT/common/logger"
#三方库
SPDLOG_INCLUDE="$PROJECT_ROOT/third_party/spdlog/include"
ETCD_INCLUDE="$PROJECT_ROOT/third_party/etcd-cpp-apiv3"
#输出地址
TEST_DIR="$PROJECT_ROOT/common/amo_etcd/test"
OUTPUT_DIR="$TEST_DIR"

# -----------------------
# 编译 test 可执行文件：speecher用到了日志模块，链接日志模块的库amo_log.a进行编译
# -----------------------
echo "Building test executable..."
g++ -g -O0 -std=c++11 \
    -I$LOGGER_INCLUDE \
    -I$AMO_ETCD_INCLUDE \
    -I$SPDLOG_INCLUDE \
    -I$ETCD_INCLUDE \
    $TEST_DIR/*.cpp \
    -L$LOGGER_LIB -lamo_log \
    -L. -letcd-cpp-api -lcpprest -lboost_system -pthread \
    -o $OUTPUT_DIR/test

echo "Build success: $OUTPUT_DIR/test"
