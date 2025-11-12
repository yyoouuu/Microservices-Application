# 包含头文件目录（上层include）
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# 收集测试源文件
file(GLOB TEST_SRC "*.cpp")  # 如main.cpp

# 编译测试可执行文件
add_executable(logger_test ${TEST_SRC})

# 链接日志库
target_link_libraries(logger_test amo_log)

# 指定可执行文件输出目录（可选）
set_target_properties(logger_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
