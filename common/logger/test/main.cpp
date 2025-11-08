#include "../src/logger.h" // 你的 Logger 定义头文件
#include <thread>
#include <chrono>

using namespace HSDK;
//方法1 (默认日志记录器，直接使用)：
void test_default_logger()
{
    APP_TRACE("TRACE {}", "T");
    APP_DEBUG("DEBUG {}", "D");
    APP_INFO("INFO {}", "I");
    APP_WARN("WARN {}", "W");
    APP_ERROR("ERROR {}", "E");
}
//方法2 （手动初始化日志记录器）：
void test_logger()
{
    // 1. 获取单例对象
    auto log = Logger::instance();

    // 2. 添加控制台输出（带颜色）
    log->add_color_console("main_logger");

    // 3. 添加轮转文件输出
    log->add_rotating_file("main_logger", "logs/main_logger.txt");

    // 4. 初始化（默认同步模式，可改为 MODE_ASYNC 测试异步）
    log->init(MODE_SYNC);
   

    // 7. 输出不同级别日志
    LOG_TRACE("main_logger", "LOG_TRACE: {}", "abc22");
    LOG_DEBUG("main_logger", "LOG_DEBUG: {}", "abc33");
    LOG_INFO("main_logger", "LOG_INFO: {}", "abc33");
    LOG_WARN("main_logger", "LOG_WARN: {}", "abc33");
    LOG_ERROR("main_logger", "LOG_ERROR: {}", "abc33");
    LOG_CRITI("main_logger", "LOG_CRITI: {}", "abc33");
    

    // 8. 模拟多次输出，看轮转文件是否生效
    for (int i = 0; i < 10000; ++i)
    {
        LOG_INFO("main_logger", "LOG_INFO:index = {}", i);//{} 是 C++ 的类型安全格式占位符。
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    // 9. 打印结束
    LOG_INFO("main_logger","==== Logging test finished ====");
}

int main()
{
    test_default_logger();
    test_logger();
    return 0;
}