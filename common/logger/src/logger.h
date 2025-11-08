#pragma once
#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/async_logger.h"
#include "spdlog/details/thread_pool.h"
#include "spdlog/details/thread_pool-inl.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "logger_def.h"
#include "singleton.hpp"

namespace HSDK {
	class Logger : public Singleton<Logger>//在用Logger构建对象时自动生成单例
	{
	public:
		Logger();
		~Logger();

		/***
		 * @description: 控制台
		 * @param pLoggerName 	记录器名称
		 * @param level 		日志输出等级
		 * @param format 		日志输出格式
		 * @return {*}
		 */
		bool add_color_console(const std::string& logger_name, 
			//默认值
			const int& level = LEVEL_TRACE, const std::string& format = LOG_OUTPUT_FORMAT);

		/***
		 * @description: 旋转文件 （到了限定的大小就会重新生成新的文件）
		 * @param pLoggerName	记录器名称
		 * @param pFileName		日志名称
		 * @param nMaxFileSize	生成的文件内容最大容量，单位byte
		 * @param nMaxFile		生成的文件最大数量
		 * @param level			日志输出等级
		 * @param format 		日志输出格式
		 * @return {*}
		 */
		bool add_rotating_file(const std::string& logger_name, const std::string& file_name,
			//默认值文件最大10mb,文件个数最大100
			const int max_file_size = 1024 * 1024 * 10, const int max_file = 100,
			const int& level = LEVEL_TRACE, const std::string& format = LOG_OUTPUT_FORMAT);

		/***
		 * @description: 日期文件（在每天的指定时间生成一个日志文件, 文件名以日期命名, 当天如果没文件会立即创建一个）
		 * @param pLoggerName	记录器名称
		 * @param pFileName		日志名称
		 * @param nHour			指定生成时间的时
		 * @param nMinute		指定生成时间的分
		 * @param eLevel		日志输出等级
		 * @param format 		日志输出格式
		 * @return {*}
		 */
		bool add_daily_file(
			const std::string& logger_name, const std::string& file_name,
			const int hour, const int minute,
			//默认值
			const int& level = LEVEL_TRACE, const std::string& format = LOG_OUTPUT_FORMAT);

		/***
		 * @description: 日期旋转文件（在每天的指定时间生成一个日期目录, 内部日志文件到了限定的大小就会重新生成新的文件）
		 * @param logger_name	记录器名称
		 * @param base_dir		日志基础目录
		 * @param file_name		日志名称
		 * @param hour			指定生成时间的时
		 * @param minute		指定生成时间的分
		 * @param max_file_size	生成的文件内容最大容量，单位byte
		 * @param max_files		生成的文件最大数量
		 * @param level			日志输出等级
		 * @param format 		日志输出格式
		 * @return {*}
		 */
		bool add_daily_rotating_file(
			const std::string& logger_name, const std::string& base_dir, const std::string& file_name,
			//默认值
			int hour = 0, int minute = 0, size_t max_file_size = 1024 * 1024 * 10, size_t max_files = 10,
			const int& level = LEVEL_TRACE, const std::string& format = LOG_OUTPUT_FORMAT);

		/**
		 * @brief 初始化日志
		 * @param out_mode 日志输出模式（同步/异步）
		 * @return 成功返回true，失败返回false
		 */
		bool init(const int& out_mode = MODE_SYNC);


		std::shared_ptr<spdlog::logger> get_logger(const std::string& name);

	private:
		//更新记录器需要的sink容器
		void update_sink_map(std::string logger_name, spdlog::sink_ptr sink);
		//更新异步记录器需要的线程池
		void update_thread_pool_map(std::string logger_name, std::shared_ptr<spdlog::details::thread_pool> thread_pool);

	protected:
		friend class Singleton<Logger>;

		//<logger名称，logger需要初始化的sinks>：存储初始化前的sink（存在一个logger有多个sink，且有多个logger的情况）
		std::map<std::string, std::vector<spdlog::sink_ptr>> map_logger_param_;
		//<logger名称，logger异步需要的线程池>：由于记录器获取线程池的weak_ptr，所以线程池对象必须比记录器对象的寿命长
		std::map<std::string, std::shared_ptr<spdlog::details::thread_pool>> map_async_thread_pool_;
	};

};