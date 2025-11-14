cmake_minimum_required(VERSION 3.10)
project(speech_server_test)

# -------------------------------
# 头文件目录
# -------------------------------
#自身头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
# logger 头文件
include_directories(/home/amoureux/amoureux/Microservices/common/logger/include)
#amo_etcd 头文件
include_directories(/home/amoureux/amoureux/Microservices/common/amo_etcd/include)
# brpc 头文件
include_directories(/home/amoureux/amoureux/Microservices/third_party/brpc/build/output/include)
# speech 头文件
include_directories(/home/amoureux/amoureux/Microservices/common/speech/include)

include_directories(/usr/include/jsoncpp)

# -------------------------------
# 查找依赖库
# -------------------------------
find_package(spdlog REQUIRED)      # 仅用于 header-only，实际不会链接
find_package(fmt REQUIRED)
find_package(etcd-cpp-api REQUIRED)  # etcd-cpp-apiv3 必须已安装
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
link_directories(
    /home/amoureux/amoureux/Microservices/third_party/brpc/build/output/lib
)

# 链接 logger 静态库
link_directories(/home/amoureux/amoureux/Microservices/common/logger/lib)

# -------------------------------
# 收集源文件
# -------------------------------
# 当前目录中的测试文件
file(GLOB TEST_SRC "*.cpp")
# src 目录中的实现文件
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cc")
# 合并
list(APPEND TEST_SRC ${SRC_FILES})


# -------------------------------
# 编译测试可执行文件
# -------------------------------
add_executable(speech_server_test ${TEST_SRC})

# 链接依赖
target_link_libraries(speech_server_test
    PRIVATE
    #logger
    amo_log            # 你自己编译的 logger 库（header-only spdlog）
    fmt::fmt           # fmt，供 spdlog 使用
    #speecher
    curl
    ssl
    jsoncpp
    etcd-cpp-api       # etcd-cpp-apiv3 库
    gRPC::grpc++       # grpc
    protobuf::libprotobuf  # protobuf
    brpc
    gflags
    leveldb
)

# 可执行文件输出目录
set_target_properties(speech_server_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)