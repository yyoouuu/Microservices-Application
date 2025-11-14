cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
# -------------------------------
# 头文件目录
# -------------------------------
# speech 自身头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
# logger 头文件
include_directories(/home/amoureux/amoureux/Microservices/common/logger/include)

include_directories(/usr/include/jsoncpp)
# -------------------------------
# 查找依赖库
# -------------------------------
find_package(spdlog REQUIRED)      # 仅用于 header-only，实际不会链接
find_package(fmt REQUIRED)
# 链接 logger 静态库
link_directories(/home/amoureux/amoureux/Microservices/common/logger/lib)

# -------------------------------
# 收集源文件
# -------------------------------
file(GLOB TEST_SRC "*.cpp")

# -------------------------------
# 编译测试可执行文件
# -------------------------------
add_executable(test_speech ${TEST_SRC})


# 链接依赖
target_link_libraries(test_speech
    PRIVATE
    amo_log            # 你自己编译的 logger 库（header-only spdlog）
    fmt::fmt           # fmt，供 spdlog 使用
    curl
    crypto
    jsoncpp
)

# 可执行文件输出目录
set_target_properties(test_speech PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)