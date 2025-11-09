#pragma once
#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <chrono>
#include <string>
#include <mutex>

namespace spdlog {
namespace sinks {

class daily_rotating_file_sink : public spdlog::sinks::base_sink<std::mutex> {
public:
    daily_rotating_file_sink(const std::string& base_path, const std::string& file_name, size_t max_size, size_t max_files, int hour, int minute);

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override;
    void flush_() override;

private:
    bool create_directory(const std::string& path);

private:
    std::string base_path_;
    std::string file_name_;
    size_t max_size_;
    size_t max_files_;
    int hour_;
    int minute_;
    std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> rotating_sink_;
    std::string current_directory_;

    bool rotation_required(std::chrono::system_clock::time_point current_time);
    void set_daily_directory();
    void create_rotating_file_sink();
};

} // namespace sinks
} // namespace spdlog