#!/bin/bash
set -e

# -----------------------
# 配置路径
#include <brpc/server.h>
#include "speech_recognizer.h" // 语音识别模块封装
#include "amo_etcd.h"          // 服务注册模块封装
#include "logger.h"            // 日志模块封装
#include "speech.pb.h"         // protobuf框架代码
#include "SpeechService.h"
#include <gflags/gflags.h>
# -----------------------
# -----------------------
# 二次封装头文件路径
# -----------------------
project_root="/home/amoureux/amoureux/Microservices"
rpc_server_include="$project_root/third_party/brpc/build/output/include"
speech_recognizer_include="$project_root/common/speech/include"
speech_recognizer_src="$project_root/common/speech/src"
logger_src="$project_root/common/logger/src"
logger_include="$project_root/common/logger/include"
amo_etcd_include="$project_root/common/amo_etcd/include"
amo_etcd_src="$project_root/common/amo_etcd/src"
speech_proto_include="$project_root/business_cluster/speech_server"
speech_proto_src="$project_root/business_cluster/speech_server"
SpeechService_include="$project_root/business_cluster/speech_server/include"
SpeechService_src="$project_root/business_cluster/speech_server/src"
# -----------------------
# 二次封装库
# -----------------------
logger_lib="$project_root/common/logger"
# -----------------------
# 第三方库
# -----------------------
spdlog_include="$project_root/third_party/spdlog/include"
etcd_include="$project_root/third_party/etcd-cpp-apiv3"
rpc_server_lib="$project_root/third_party/brpc/output/lib/"

# -----------------------
# 输出地址
# -----------------------
test_dir="$project_root/business_cluster/speech_server/test"
output_dir="$test_dir"

# -----------------------
# 编译 test 可执行文件
# -----------------------
echo "building test executable..."
g++ -g -O0 -std=c++11 \
    -I/usr/include \
    -I$logger_include \
    -I$amo_etcd_include \
    -I$spdlog_include \
    -I$etcd_include \
    -I$speech_recognizer_include \
    -I$speech_proto_include \
    -I$SpeechService_include \
    -I$rpc_server_include \
    -I/usr/include/jsoncpp \
    $speech_proto_src/speech.pb.cc \
    $test_dir/*.cpp \
    -L$logger_lib -lamo_log \
    /home/amoureux/amoureux/Microservices/third_party/brpc/build/output/lib/libbrpc.a \
    -lgflags -letcd-cpp-api -labsl_synchronization -lcpprest -lboost_system -lcurl -ljsoncpp -lprotobuf \
    -lssl -lcrypto -lleveldb -lz -lpthread -ldl -lrt -labsl_base -labsl_synchronization\
    -o $output_dir/test

echo "build success: $output_dir/test"