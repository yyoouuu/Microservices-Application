#pragma once
#include "SpeechServiceImpl.h"
#include "amo_etcd"
class SpeechServerBuilder
{
private:
    // 语音识别对象
    speech::SpeechRecognizer::s_ptr _speechRecognizer;

    // 服务注册对象
    grpc_ns::Registry::s_ptr _register;

    // rpc服务对象

public:
    void make_speecher_object(const std::string &app_id,
                              const std::string &api_key,
                              const std::string &secret_key)
    {
        _speechRecognizer = std::make_shared<speech::SpeechRecognizer>(app_id, api_key, secret_key);
    }

    void make_reg_object(const std::string &host_name, const std::string &service_name, const std::string &access_host)
    {
        _register = std::make_shared<grpc_ns::Registry>(host_name, 3);
        _register.register(service_name,access_host);
    }
    
}