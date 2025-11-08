#pragma once
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

namespace HSDK {

	//默认的日志输出格式
	//#define LOG_OUTPUT_FORMAT      "[%Y-%m-%d %H:%M:%S.%e] [%s,%#][%!] [%t] %^[%l]%$: %v"
#define LOG_OUTPUT_FORMAT      "[%Y-%m-%d %H:%M:%S.%e] [%s,%#] [%t] %^[%l]%$: %v"	//去除函数名称

/* 日志输出模式 */
	static const int MODE_SYNC = 0;
	static const int MODE_ASYNC = 1;

	/* 日志输出等级 */
	static const int LEVEL_TRACE = 0;
	static const int LEVEL_DEBUG = 1;
	static const int LEVEL_INFO = 2;
	static const int LEVEL_WARN = 3;
	static const int LEVEL_ERROR = 4;
	static const int LEVEL_CRITI = 5;
	static const int  LEVEL_OFF = 6;


	//封装宏，没有该宏无法输出文件名、行号等信息
#define LOG_TRACE(loggerName, ...) SPDLOG_LOGGER_CALL(Logger::instance()->get_logger(loggerName), spdlog::level::trace, __VA_ARGS__)
#define LOG_DEBUG(loggerName, ...) SPDLOG_LOGGER_CALL(Logger::instance()->get_logger(loggerName), spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(loggerName, ...) SPDLOG_LOGGER_CALL(Logger::instance()->get_logger(loggerName), spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(loggerName, ...) SPDLOG_LOGGER_CALL(Logger::instance()->get_logger(loggerName), spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(loggerName, ...) SPDLOG_LOGGER_CALL(Logger::instance()->get_logger(loggerName), spdlog::level::err, __VA_ARGS__)
#define LOG_CRITI(loggerName, ...) SPDLOG_LOGGER_CALL(Logger::instance()->get_logger(loggerName), spdlog::level::critical, __VA_ARGS__)

#define APP_TRACE(...) 	LOG_TRACE("default_logger", __VA_ARGS__)  
#define APP_DEBUG(...) 	LOG_DEBUG("default_logger", __VA_ARGS__)  
#define APP_INFO(...) 	LOG_INFO("default_logger", __VA_ARGS__)   
#define APP_WARN(...) 	LOG_WARN("default_logger", __VA_ARGS__)   
#define APP_ERROR(...) 	LOG_ERROR("default_logger", __VA_ARGS__)  
#define APP_CRITI(...) 	LOG_CRITI("default_logger", __VA_ARGS__)  

};