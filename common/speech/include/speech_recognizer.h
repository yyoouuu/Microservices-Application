/**
 * @file: common/speech/include/speech_recognizer.h
 * @brief: 对百度语音识别模块的二次封装
 * @namespace: speech
 * @copyright: Copyright (c) 2023 CompanyName
 * @note: 依赖 speech_recognition/speech.h 和 logger/src/logger.h
 *
 * @warning 使用头文件声明和源文件定义的形式，二次封装语音识别库会造成一些函数的multiple definition
 *          解决方式：直接在头文件中定义
 * @authors: 吴正友
 * @date: 2025-11-8
 */

#pragma once
#include "../../../third_party/speech_recognition/speech.h"
#include "../../logger/src/logger.h"     //  先包含类定义
#include "../../logger/src/logger_def.h" // 再包含宏定义
#include <memory>
/**
 * 语音识别器
 *
 * @note 采用智能指针管理,程序结束自动销毁
 *
 * 使用示例：
 *
 *     auto speechrecognizer = std::make_shared<speech::SpeechRecognizer>(
 *      "120716329",                      // 百度语音应用 AppID
 *      "4miJFxqVYsePhbzcz5zmXiW9",       // API Key
 *      "N3SpF7ELheRyYiMN2mk2zJWHtWBAATzQ" // Secret Key
 *
 *
 *  const char* file_path = "/home/amoureux/amoureux/Microservices/third_party/speech_recognition/test/speech/assets/16k_test.pcm";
 *  std::string recognition_result = speechrecognizer->recognize(file_path);
 *
 */

namespace speech_ns
{
    class SpeechRecognizer
    {

    public:
        using s_ptr = std::shared_ptr<SpeechRecognizer>;
        /**
         * 构造函数
         *
         * @param [in] app_id 百度语音应用 AppID
         * @param [in] api_key 百度语音应用 API Key
         * @param [in] secret_key 百度语音应用 Secret Key
         *
         * @pre 已想百度智能云获取三参
         *
         * @example
         * auto speechrecognizer = std::make_shared<speech::SpeechRecognizer>(
         * "120716329",                      // 百度语音应用 AppID
         * "4miJFxqVYsePhbzcz5zmXiW9",       // API Key
         * "N3SpF7ELheRyYiMN2mk2zJWHtWBAATzQ" // Secret Key
         * );
         */
        SpeechRecognizer(
            const std::string &app_id,
            const std::string &api_key,
            const std::string &secret_key):_client(app_id,api_key,secret_key)
        {
        }
        /**
         * 识别音频文件
         *
         * @param [in] file_path 音频文件地址(全局或相对地址)
         * @param [in] format 音频文件格式
         * @param [in] sample_rate 采样率
         *
         * @return std::string
         * - 成功：返回识别出的文本内容；
         * - 失败：返回错误信息字符串；
         * - 无识别结果：返回 "No recognition result."
         *
         * @example
         * const char* file_path = "/home/amoureux/amoureux/Microservices/third_party/speech_recognition/test/speech/assets/16k_test.pcm";
         * std::string recognition_result = speechrecognizer->recognize(file_path);
         *
         */
        std::string recognize(const char *file_path,
                              const std::string &format = "pcm",
                              int sample_rate = 16000)
        {
            std::string file_content;
            aip::get_file_content(file_path, &file_content);

            std::map<std::string, std::string> options; // 可设置可选参数
            Json::Value result_json = _client->recognize(file_content, format, sample_rate, options);

            // 判断百度返回错误
            if (result_json.isMember("err_no") && result_json["err_no"].asInt() != 0)
            {
                APP_ERROR("Error:", result_json["err_msg"].asString());
                return "Error: " + result_json["err_msg"].asString();
            }

            // 判断 result_json["result"][0].asString() 数组是否存在且非空
            if (result_json.isMember("result") && result_json["result"].isArray() && !result_json["result"].empty())
            {
                APP_INFO("recognition_result: {}", result_json["result"][0].asString());
                return result_json["result"][0].asString(); // 返回第一个识别结果
            }

            return "";
        }

    private:
        aip::Speech _client;
    };
}