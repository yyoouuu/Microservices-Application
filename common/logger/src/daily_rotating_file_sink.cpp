#include "daily_rotating_file_sink.h"
#include <spdlog/details/os.h>
#include <spdlog/details/file_helper.h>
#include <iostream>
#include <filesystem>
#include <ctime>

/* 创建目录的宏 */
#if defined(_WIN32)
#include <direct.h>   // 用于 _mkdir 函数
#include <sys/stat.h> // 用于 _stat 函数
#define MKDIR(path) _mkdir(path)
#define STAT _stat
struct stat_struct : public _stat {};
#else
#include <sys/stat.h> // 用于 mkdir 和 stat 函数
#include <unistd.h>   // POSIX 系统的标准库
#define MKDIR(path) mkdir(path, 0755)
#define STAT stat
struct stat_struct : public stat {};
#endif

namespace spdlog {
namespace sinks {

daily_rotating_file_sink::daily_rotating_file_sink(const std::string& base_path, const std::string& file_name, size_t max_size, size_t max_files, int hour, int minute)
    : base_path_(base_path), file_name_(file_name), max_size_(max_size), max_files_(max_files), hour_(hour), minute_(minute)
{
    set_daily_directory();
    create_rotating_file_sink();
}

void daily_rotating_file_sink::sink_it_(const spdlog::details::log_msg& msg)
{
    auto time_now = spdlog::details::os::now();
    if (rotation_required(time_now)) {
        set_daily_directory();
        create_rotating_file_sink();
    }

    rotating_sink_->log(msg);
}

void daily_rotating_file_sink::flush_()
{
    rotating_sink_->flush();
}

bool daily_rotating_file_sink::create_directory(const std::string& path)
{
    // 检查目录是否已经存在
    stat_struct info;
    if (STAT(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR)) {
        std::cout << "Directory already exists: " << path << std::endl;
        return false;
    }

    // 尝试创建目录
    if (MKDIR(path.c_str()) == 0) {
        std::cout << "Directory created successfully: " << path << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to create directory: " << path << std::endl;
        return false;
    }
}

bool daily_rotating_file_sink::rotation_required(std::chrono::system_clock::time_point current_time)
{
    std::time_t t = std::chrono::system_clock::to_time_t(current_time);
    std::tm now_tm = *std::localtime(&t);

    return (now_tm.tm_hour == hour_ && now_tm.tm_min >= minute_);
}

void daily_rotating_file_sink::set_daily_directory()
{
    auto time_now = spdlog::details::os::now();
    std::time_t t = std::chrono::system_clock::to_time_t(time_now);
    std::tm now_tm = *std::localtime(&t);

    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &now_tm);

    current_directory_ = base_path_ + "/" + buffer;
    create_directory(current_directory_);
}

void daily_rotating_file_sink::create_rotating_file_sink()
{
    std::string file_name = current_directory_ + "/" + file_name_;
    rotating_sink_ = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(file_name, max_size_, max_files_);
}

} // namespace sinks
} // namespace spdlog