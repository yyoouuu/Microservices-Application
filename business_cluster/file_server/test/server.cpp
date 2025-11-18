#include "FileServer.h"
#include <gflags/gflags.h>

DEFINE_string(log_name, "main_logger", "发布模式下，用于指定日志的输出文件");
DEFINE_string(log_file, "test_log", "发布模式下，用于指定日志的输出文件");


DEFINE_string(registry_host, "http://127.0.0.1:2379", "服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(instance_name, "/file_service/instance", "当前实例名称");
DEFINE_string(access_host, "127.0.0.1:10002", "当前实例的外部访问地址");

DEFINE_int32(listen_port, 10002, "Rpc服务器监听端口");
DEFINE_int32(rpc_timeout, -1, "Rpc调用超时时间");
DEFINE_int32(rpc_threads, 1, "Rpc的IO线程数量");

DEFINE_string(storage_path, "../storage_file", "存储文件目录");

int main(int argc, char *argv[])
{
    //读取配置
    google::ParseCommandLineFlags(&argc, &argv, true);
    //初始化日志系统
    auto log = HSDK::Logger::instance();
    log->init(FLAGS_log_name, FLAGS_log_file);

    //语音识别服务构建
    amo_file_server::FileServerBuiler fsb;

    //初始化RPC服务器模块
    fsb.make_rpc_server(FLAGS_listen_port, FLAGS_rpc_timeout, FLAGS_rpc_threads, FLAGS_storage_path);
    //初始化服务注册模块
    fsb.make_reg_object(FLAGS_registry_host, FLAGS_base_service + FLAGS_instance_name, FLAGS_access_host);
    
    //构建RPC语音识别服务
    auto server = fsb.build();
    //服务启动
    server->start();
    return 0;
}