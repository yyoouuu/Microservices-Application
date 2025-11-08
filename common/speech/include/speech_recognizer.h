/*
    对百度语音识别模块的二次封装
*/
#pragma once
#include "../../../third_party/speech_recognition/speech.h"
#include <memory>

// 填写appid: 120716329
// Api Key: 4miJFxqVYsePhbzcz5zmXiW9
// Secret Key: N3SpF7ELheRyYiMN2mk2zJWHtWBAATzQ

namespace speech
{
    class SpeechRecognizer
    {

    public:
        using s_ptr = std::shared_ptr<SpeechRecognizer>;
        // 构造函数
        SpeechRecognizer(
            const std::string &app_id,
            const std::string &api_key,
            const std::string &secret_key){
        _client = new aip::Speech(app_id, api_key, secret_key);
        _client->setDebug(false); // 设置是否打印调试信息
    }
        // 识别本地音频文件
        std::string recognize(const char* file_path,
                                  const std::string &format = "pcm",
                                  int sample_rate = 16000){
        std::string file_content;
        aip::get_file_content(file_path, &file_content);
   
        std::map<std::string, std::string> options; // 可设置可选参数
        Json::Value result_json = _client->recognize(file_content, format, sample_rate, options);

        // 判断百度返回错误
        if (result_json.isMember("err_no") && result_json["err_no"].asInt() != 0)
        {
            return "Error: " + result_json["err_msg"].asString();
        }

        // 判断 result 数组是否存在且非空
        if (result_json.isMember("result") && result_json["result"].isArray() && !result_json["result"].empty())
        {
            return result_json["result"][0].asString(); // 返回第一个识别结果
        }

        return "No recognition result.";
    }

    private:
        aip::Speech *_client;
    };
}