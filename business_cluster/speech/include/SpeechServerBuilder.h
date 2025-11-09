#pragma
#include"speech_recognizer.h"

class SpeechServerBuilder
{
    private:
    //语音识别对象
    speech::SpeechRecognizer::s_ptr _speechRecognizer;
    //rpc服务对象

    //服务注册对象

    public:

    void make_speecher_object(const std::string &app_id,
            const std::string &api_key,
            const std::string &secret_key):_speechRecognizer(app_id,api_key,secret_key)
            {
                
            }

}