//主要实现语音识别子服务的服务器的搭建
#include "SpeechService.h"
#include <butil/logging.h>
#include <gflags/gflags.h>

DEFINE_string(log_name, "main_logger", "发布模式下，用于指定日志的输出文件");
DEFINE_string(log_file, "test_log", "发布模式下，用于指定日志的输出文件");

DEFINE_string(registry_host, "http://127.0.0.1:2379", "服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(instance_name, "/speech_service/instance", "当前实例名称");
DEFINE_string(access_host, "127.0.0.1:10001", "当前实例的外部访问地址");

DEFINE_int32(listen_port, 10001, "Rpc服务器监听端口");
DEFINE_int32(rpc_timeout, -1, "Rpc调用超时时间");
DEFINE_int32(rpc_threads, 1, "Rpc的IO线程数量");

DEFINE_string(app_id, "120716329", "语音平台应用ID");
DEFINE_string(api_key, "4miJFxqVYsePhbzcz5zmXiW9", "语音平台API密钥");
DEFINE_string(secret_key, "N3SpF7ELheRyYiMN2mk2zJWHtWBAATzQ", "语音平台加密密钥");


int main(int argc, char *argv[])
{
    //读取配置
    google::ParseCommandLineFlags(&argc, &argv, true);
    //初始化日志系统
    auto log = HSDK::Logger::instance();
    log->init(FLAGS_log_name, FLAGS_log_file);

    //语音识别服务构建
    amo_speech_server::SpeechServerBuilder ssb;
    //创建 RPC 服务端
    ssb.make_speecher_object(FLAGS_app_id, FLAGS_api_key, FLAGS_secret_key);
    //rpc服务对象构建
    ssb.make_rpc_server(FLAGS_listen_port, FLAGS_rpc_timeout, FLAGS_rpc_threads);
    //注册服务信息
    ssb.make_reg_object(FLAGS_registry_host, FLAGS_base_service + FLAGS_instance_name, FLAGS_access_host);
    auto server = ssb.build();
    server->start();

    return 0;
}

