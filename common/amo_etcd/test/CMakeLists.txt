cmake_minimum_required(VERSION 3.10)
project(amo_etcd_test)

# -------------------------------
# 头文件目录
# -------------------------------
# amo_etcd 自身头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
# logger 头文件
include_directories(/home/amoureux/amoureux/Microservices/common/logger/include)

# -------------------------------
# 查找依赖库
# -------------------------------
find_package(spdlog REQUIRED)      # 仅用于 header-only，实际不会链接
find_package(fmt REQUIRED)
find_package(etcd-cpp-api REQUIRED)  # etcd-cpp-apiv3 必须已安装
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# 链接 logger 静态库
link_directories(/home/amoureux/amoureux/Microservices/common/logger/lib)

# -------------------------------
# 收集源文件
# -------------------------------
file(GLOB TEST_SRC "*.cpp")  # 例如 test_amo_etcd.cpp

# -------------------------------
# 编译测试可执行文件
# -------------------------------
add_executable(test_amo_etcd ${TEST_SRC})

# 链接依赖
target_link_libraries(test_amo_etcd
    PRIVATE
    amo_log            # 你自己编译的 logger 库（header-only spdlog）
    etcd-cpp-api       # etcd-cpp-apiv3 库
    gRPC::grpc++       # grpc
    protobuf::libprotobuf  # protobuf
    fmt::fmt           # fmt，供 spdlog 使用
)

# 可执行文件输出目录
set_target_properties(test_amo_etcd PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)