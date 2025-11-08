# 设置 CMake 的最低版本要求为 3.10
cmake_minimum_required(VERSION 3.10)
# 定义工程名称为 test，版本号为 1.0
project(test VERSION 1.0)



# 添加 include 搜索路径，将当前工程根目录加入头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR})
# 搜索当前工程根目录下所有的 .h 和 .cpp 文件，并存入 SOURCES 变量
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/*.h 
    ${PROJECT_SOURCE_DIR}/*.cpp
)

#指定可执行文件输出路径

# 定义可执行文件目标 test，源文件为 SOURCES 中的所有文件
add_executable(test ${SOURCES})
set_target_properties(test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "/home/amoureux/amoureux/Microservices/project-root/common/logger/test"
)

# 将 test 可执行文件链接 libspdlog 静态库
# PUBLIC 表示 test 链接 libspdlog 后，libspdlog 的头文件路径也会被传播给依赖它的目标
target_link_libraries(test PUBLIC libspdlog)

