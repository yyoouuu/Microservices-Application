#logger
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
project(logger_test)

# 包含头文件目录
include_directories(include)

# 查找依赖库（spdlog 和 fmt）
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

# 收集源文件
file(GLOB LOGGER_SRC "src/*.cpp")

# 编译静态库（链接 spdlog 和 fmt）
add_library(amo_log STATIC ${LOGGER_SRC})
target_compile_definitions(amo_log PUBLIC SPDLOG_HEADER_ONLY)
target_link_libraries(amo_log PRIVATE fmt::fmt)

# 指定库的输出目录
set_target_properties(amo_log PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# 添加测试子模块
add_subdirectory(test)


