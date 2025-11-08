# 设置 CMake 的最低版本要求为 3.10
cmake_minimum_required(VERSION 3.10)

# 定义工程名称为 libspdlog，版本号为 1.0
project(libspdlog VERSION 1.0)

# 开启 Debug 模式
set(CMAKE_BUILD_TYPE Debug)

# 添加 include 搜索路径，将当前工程 src 目录加入头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/src)
# 搜索 src 目录下所有的 .h 和 .cpp 文件，并存入 SOURCES 变量
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.h 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# 添加 spdlog 第三方库作为子目录，指定二进制输出目录 spdlog_build
# 注意：这里前提是 spdlog 源码目录里必须包含 CMakeLists.txt
add_subdirectory("/home/amoureux/amoureux/Microservices/third_party/spdlog" spdlog_build)

# 定义一个静态库 libspdlog，源文件为之前搜集的 SOURCES
add_library(libspdlog STATIC ${SOURCES})

# 将 libspdlog 链接 spdlog 的 header-only 目标库
# PUBLIC 表示使用 libspdlog 的目标也可以访问 spdlog 的头文件
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog_header_only)

# 添加 test 子目录，构建测试程序
# 前提是 test 目录下有自己的 CMakeLists.txt
add_subdirectory(test)


