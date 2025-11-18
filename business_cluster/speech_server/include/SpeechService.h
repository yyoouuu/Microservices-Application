#pragma once
#include <brpc/server.h>
#include <butil/logging.h>
#include "speech_recognizer.h" // 语音识别模块封装
#include "amo_etcd.h"          // 服务注册模块封装
#include "logger.h"            // 日志模块封装
#include "speech.pb.h"         // protobuf框架代码

namespace amo_speech_server
{
    // 语音识别实现类，语音识别器识别request，将识别结果填充回response
    class SpeechServiceImpl : public mic_ser::SpeechService
    {
    private:
        speech_ns::SpeechRecognizer::s_ptr _speechRecognizer;

    public:
        SpeechServiceImpl(speech_ns::SpeechRecognizer::s_ptr speechRecognizer) : _speechRecognizer(speechRecognizer)
        {
        }
        void SpeechRecognition(google::protobuf::RpcController *controller,
                               const ::mic_ser::SpeechRecognitionReq *request, // 客户端发送的请求消息
                               ::mic_ser::SpeechRecognitionRsp *response,      // 服务器填充响应内容的对象，最后返回给客户端
                               ::google::protobuf::Closure *done)
        {
            brpc::ClosureGuard rpc_guard(done);
            APP_INFO("收到语音转文字请求！");
            // 1. 取出请求中的语音数据
            // 2. 调用语音sdk模块进行语音识别，得到响应
            std::string recognition_result = _speechRecognizer->recognize(request->speech_datas().c_str());
            if (recognition_result.empty())
            {
                std::string err = "识别为空";
                response->set_response_id(request->request_id());
                response->set_success(false);
                response->set_errmsg(err);
                return;
            }
            // 组织响应
            response->set_response_id(request->request_id());
            response->set_success(true);
            response->set_recognition_result(recognition_result);
        }
        ~SpeechServiceImpl() {}
    };

    // 语音识别子服务启动类
    class SpeechServer
    {
    private:
        // 语音识别对象
        speech_ns::SpeechRecognizer::s_ptr _speechRecognizer;

        // 服务注册对象
        etcd_ns::Registry::s_ptr _register;

        // rpc服务对象
        std::shared_ptr<brpc::Server> _rpc_server;

    public:
        using s_ptr = std::shared_ptr<SpeechServer>;
        SpeechServer(speech_ns::SpeechRecognizer::s_ptr speechRecognizer,
                     etcd_ns::Registry::s_ptr registry,
                     std::shared_ptr<brpc::Server> &rpc_server) : _speechRecognizer(speechRecognizer), _register(registry), _rpc_server(rpc_server)
        {
        }

        // 搭建RPC服务器，并启动服务器
        void start()
        {
            _rpc_server->RunUntilAskedToQuit();
            APP_INFO("rpc_server启动成功");
        }

        ~SpeechServer()
        {
        }
    };

    // 语音识别子服务实例构建类：以前置参数，创建三个语音识别所需要的实例，语音识别对象实例，服务注册对象实例，rpc请求对象实例
    class SpeechServerBuilder
    {
    private:
        // 语音识别对象
        speech_ns::SpeechRecognizer::s_ptr _speechRecognizer;

        // 服务注册对象
        etcd_ns::Registry::s_ptr _register;

        // rpc服务对象
        std::shared_ptr<brpc::Server> _rpc_server;

    public:
        void make_speecher_object(const std::string &app_id,
                                  const std::string &api_key,
                                  const std::string &secret_key)
        {
            _speechRecognizer = std::make_shared<speech_ns::SpeechRecognizer>(app_id, api_key, secret_key);
        }

        void make_reg_object(const std::string &host_name, const std::string &service_name, const std::string &access_host)
        {
            _register = std::make_shared<etcd_ns::Registry>(host_name, 3);
            _register->registry(service_name, access_host);
        }
        void make_rpc_server(uint16_t port, int32_t timeout, uint8_t num_threads)
        {
            if (!_speechRecognizer)
            {
                APP_ERROR("还未初始化语音识别模块！");
                abort();
            }
            _rpc_server = std::make_shared<brpc::Server>();
            SpeechServiceImpl *speech_service = new SpeechServiceImpl(_speechRecognizer);
            int ret = _rpc_server->AddService(speech_service, brpc::ServiceOwnership::SERVER_OWNS_SERVICE);
            if (ret == -1)
            {
                APP_ERROR("添加Rpc服务失败！");
                abort();
            }
            else
            {
                APP_INFO("添加Rpc服务成功！");
            }

            brpc::ServerOptions options;
            
            options.idle_timeout_sec = timeout;
            options.num_threads = num_threads;
            ret = _rpc_server->Start(port, &options);
            if (ret == -1)
            {
                APP_ERROR("服务启动失败！");
                abort();
            }
            else
            {
                APP_INFO("服务启动成功！");
            }
        }
        SpeechServer::s_ptr build()
        {
            if (!_speechRecognizer)
            {
                APP_ERROR("还未初始化语音识别模块！");
                abort();
            }

            if (! _register)
            {
                APP_ERROR("还未初始化服务注册模块！");
                abort();
            }

            if (!_rpc_server)
            {
                APP_ERROR("还未初始化RPC服务器模块！");
                abort();
            }

            SpeechServer::s_ptr server = std::make_shared<SpeechServer>(
                _speechRecognizer,  _register, _rpc_server);
            APP_INFO("语音识别子服务实例构建成功！");
            
            return server;
        }
    };
} // namespace
