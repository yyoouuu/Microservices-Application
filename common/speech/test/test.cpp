#include "speech_recognizer.h"

/**
 * @file    test.cpp
 * @brief   示例程序：使用百度语音 SDK 进行本地 PCM 音频文件识别
 * @details
 * 该示例演示如何：
 *  1. 创建 SpeechRecognizer 对象，使用百度云申请的 AppID、API Key 和 Secret Key；
 *  2. 指定本地 PCM 音频文件路径；
 *  3. 调用 recognize 方法进行语音识别并获取结果。
 * 
 * 注意事项：
 *  - file_path 必须指向一个存在的 PCM 音频文件；
 *  - PCM 文件采样率需与 SDK 支持的采样率一致（本例为 16kHz）；
 *  - 识别结果为字符串形式，可进一步处理或显示。
 * 
 * 百度云控制台获取的 Key 信息示例：
 *  - AppID: 120716329
 *  - API Key: 4miJFxqVYsePhbzcz5zmXiW9
 *  - Secret Key: N3SpF7ELheRyYiMN2mk2zJWHtWBAATzQ
 */
int main()
{
    // 创建智能语音识别对象（共享指针管理）
    auto speechrecognizer = std::make_shared<speech_ns::SpeechRecognizer>(
        "120716329",                      // 百度语音应用 AppID
        "4miJFxqVYsePhbzcz5zmXiW9",       // API Key
        "N3SpF7ELheRyYiMN2mk2zJWHtWBAATzQ" // Secret Key
    );

    // 指定要识别的 PCM 音频文件绝对路径
    const char* file_path = "/home/amoureux/amoureux/Microservices/third_party/speech_recognition/test/speech/assets/16k_test.pcm";

    // 调用识别接口，将音频文件转换为文字
    // 返回值类型string
    std::string recognition_result = speechrecognizer->recognize(file_path);
    std::cout << recognition_result << std::endl;
    return 0;
}

