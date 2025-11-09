#pragma once
#include <brpc/server.h>
#include "../src/logger.h"
#include "speech_recognizer.h" // 语音识别模块封装
#include "etcd.hpp"            // 服务注册模块封装
#include "logger.h"            // 日志模块封装
#include "speech.pb.h"         // protobuf框架代码

class SpeechServiceImpl : public mic_ser::SpeechService
{
private:
    speech::SpeechRecognizer::s_ptr _speechRecognizer;
public:
    SpeechServiceImpl(speech::SpeechRecognizer::s_ptr &speechRecognizer) : _speechRecognizer(speechRecognizer)
    {
    }
    void SpeechRecognition(google::protobuf::RpcController *controller,
                           const ::mic_ser::SpeechRecognitionReq *request, // 客户端发送的请求消息
                           ::mic_ser::SpeechRecognitionRsp *response,      // 服务器填充响应内容的对象，最后返回给客户端
                           ::google::protobuf::Closure *done)
    {
        APP_INFO("收到语音转文字请求！");
        // 1. 取出请求中的语音数据
        // 2. 调用语音sdk模块进行语音识别，得到响应
        std::string recognition_result = speechrecognizer->recognize(request->speech_datas());
        if (recognition_result.empty())
        {
            std::string err = "识别为空";
            response->set_response_id(request->request_id);
            response->set_success(false);
            response->set_errmsg(err);
            return;
        }
        // 组织响应
        response->set_response_id(request->request_id);
        response->set_success(true);
        response->set_errmsg(recognition_result);
    }
    ~SpeechServiceImpl() {}
}