#pragma once
#include <brpc/server.h>
#include "../src/logger.h"
#include "asr.hpp"      // 语音识别模块封装
#include "etcd.hpp"     // 服务注册模块封装
#include "logger.hpp"   // 日志模块封装
#include "speech.pb.h"  // protobuf框架代码
