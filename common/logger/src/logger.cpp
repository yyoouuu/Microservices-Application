#include "logger.h"
#include "daily_rotating_file_sink.h"

using namespace HSDK;
Logger::Logger()
{
	//默认指定记录路径
	add_color_console("default_logger", LEVEL_TRACE);	
	add_rotating_file("default_logger", "logs/app.log");
	default_init();
}

Logger::~Logger()
{

}

//不同日志等级彩色输出
bool Logger::add_color_console(const std::string& logger_name, const int& level, const std::string& format)
{
	printf("[log]: addColorConsole, logName=%s  level=%d  \n", logger_name.data(), level);
	auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	consoleSink->set_level(spdlog::level::level_enum(level));
	consoleSink->set_pattern(format);
	update_sink_map(logger_name, consoleSink);
	return true;
}


bool Logger::add_rotating_file(const std::string& logger_name, const std::string& file_name, const int max_file_size, const int max_file, const int& level, const std::string& format)
{
	printf("[log]: addRotatingFile, logName=%s  level=%d  fileName=%s  maxFileSize=%d  maxFile=%d \n", logger_name.data(), level, file_name.data(), max_file_size, max_file);
	auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(file_name, max_file_size, max_file);
	file_sink->set_level(spdlog::level::level_enum(level));
	file_sink->set_pattern(format);
	update_sink_map(logger_name, file_sink);
	return true;
}


bool Logger::add_daily_file(const std::string& logger_name, const std::string& file_name, const int hour, const int minute, const int& level, const std::string& format)
{
	//"%Y-%m-%d:%H:%M:%S.log"
	printf("[log]: addDailyFile, logName=%s  level=%d  fileName=%s  hour=%d  minute=%d \n", logger_name.data(), level, file_name.data(), hour, minute);
	auto daily_file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(file_name, hour, minute);
	daily_file_sink->set_level(spdlog::level::level_enum(level));
	daily_file_sink->set_pattern(format);
	update_sink_map(logger_name, daily_file_sink);
	return true;
}

bool Logger::add_daily_rotating_file(const std::string& logger_name, const std::string& base_dir, const std::string& file_name, int hour, int minute, size_t max_file_size, size_t max_files, const int& level, const std::string& format)
{
	printf("[log]: addDailyRotatingFile, logName=%s  baseDir=%s  fileName=%s  hour=%d  minute=%d  maxFileSize=%zu  maxFile=%zu  level=%d\n", logger_name.data(), base_dir.c_str(), file_name.data(), hour, minute, max_file_size, max_files, level);
	auto daily_rotating_sink = std::make_shared<spdlog::sinks::daily_rotating_file_sink>(base_dir, file_name, max_file_size, max_files, hour, minute);
	daily_rotating_sink->set_level(spdlog::level::level_enum(level));
	daily_rotating_sink->set_pattern(format);
	update_sink_map(logger_name, daily_rotating_sink);

	return true;
}
//默认初始化
bool Logger::default_init(const int& out_mode)
{
	if (out_mode == MODE_ASYNC)//祑祭
	{
		printf("[log]: mode=ASYNC \n");
		for (auto e : map_logger_param_)
		{
			std::string Log_name = e.first;
			std::vector<spdlog::sink_ptr> vec_sink(e.second);
			auto tp = std::make_shared<spdlog::details::thread_pool>(1024000, 1);
			auto logger = std::make_shared<spdlog::async_logger>(Log_name, begin(vec_sink), end(vec_sink), tp, spdlog::async_overflow_policy::block);
			update_thread_pool_map(Log_name, tp);
			spdlog::register_logger(logger);
		}
	}
	else//肮祭
	{
		printf("[log]:  mode=SYNC \n");
		for (auto e : map_logger_param_)
		{
			std::string Log_name = e.first;
			std::vector<spdlog::sink_ptr> vec_sink(e.second);
			auto logger = std::make_shared<spdlog::logger>(Log_name, begin(vec_sink), end(vec_sink));
			spdlog::register_logger(logger);
		}
	}


	spdlog::set_level(spdlog::level::trace);
	spdlog::flush_on(spdlog::level::info);
	spdlog::flush_every(std::chrono::seconds(3));
	//spdlog::set_pattern(format);

	map_logger_param_.clear();
	return true;
}
//初始化
bool Logger::init(const std::string& logger_name, const std::string& base_dir,const int& out_mode)
{
	printf("[log]: Initializing logger: %s, base_dir=%s, mode=%s\n",
           logger_name.c_str(),
           base_dir.c_str(),
           (out_mode == MODE_ASYNC ? "ASYNC" : "SYNC"));
	 // 2. 添加控制台输出（带颜色）
    add_color_console(logger_name);

    // 3. 添加轮转文件输出
	std::string log_file_path = base_dir + "/" + logger_name + ".txt";
    add_rotating_file(logger_name, log_file_path);
	if (out_mode == MODE_ASYNC)//祑祭
	{
		printf("[log]: mode=ASYNC \n");
		for (auto e : map_logger_param_)
		{
			std::string Log_name = e.first;
			std::vector<spdlog::sink_ptr> vec_sink(e.second);
			auto tp = std::make_shared<spdlog::details::thread_pool>(1024000, 1);
			auto logger = std::make_shared<spdlog::async_logger>(Log_name, begin(vec_sink), end(vec_sink), tp, spdlog::async_overflow_policy::block);
			update_thread_pool_map(Log_name, tp);
			spdlog::register_logger(logger);
		}
	}
	else//肮祭
	{
		printf("[log]:  mode=SYNC \n");
		for (auto e : map_logger_param_)
		{
			std::string Log_name = e.first;
			std::vector<spdlog::sink_ptr> vec_sink(e.second);
			auto logger = std::make_shared<spdlog::logger>(Log_name, begin(vec_sink), end(vec_sink));
			spdlog::register_logger(logger);
		}
	}


	spdlog::set_level(spdlog::level::trace);
	spdlog::flush_on(spdlog::level::info);
	spdlog::flush_every(std::chrono::seconds(3));
	//spdlog::set_pattern(format);

	map_logger_param_.clear();
	return true;
}

std::shared_ptr<spdlog::logger> Logger::get_logger(const std::string& name)
{
	return spdlog::get(name);
}
//更新输出地址sink
void Logger::update_sink_map(std::string logger_name, spdlog::sink_ptr sink)
{
	auto iter = map_logger_param_.find(logger_name);
	if (iter != map_logger_param_.end())
	{
		iter->second.push_back(sink);
	}
	else
	{
		std::vector<spdlog::sink_ptr> vecSink;
		vecSink.push_back(sink);
		map_logger_param_[logger_name] = vecSink;
	}
}

void Logger::update_thread_pool_map(std::string logger_name, std::shared_ptr<spdlog::details::thread_pool> thread_pool)
{
	auto iter = map_async_thread_pool_.find(logger_name);
	if (iter != map_async_thread_pool_.end())
	{
		iter->second = (thread_pool);
	}
	else
	{
		map_async_thread_pool_[logger_name] = thread_pool;
	}
}