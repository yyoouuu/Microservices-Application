#pragma once
#include <string>
#include <optional>

class UserManagerService {
public:
    // 1. 用户名注册
    bool registerByUsername(const std::string& username, const std::string& password);

    // 2. 用户名登录
    bool loginByUsername(const std::string& username, const std::string& password);

    // 3. 短信验证码获取
    std::optional<std::string> requestSMSCode(const std::string& phone_number);

    // 4. 手机号注册
    bool registerByPhone(const std::string& phone_number, const std::string& sms_code);

    // 5. 手机号登录
    bool loginByPhone(const std::string& phone_number, const std::string& sms_code);

    // 6. 用户信息获取
    struct UserInfo {
        std::string username;
        std::string nickname;
        std::string phone_number;
        std::string avatar_url;
        std::string signature;
    };
    std::optional<UserInfo> getUserInfo(const std::string& username);

    // 7. 头像修改
    bool setAvatar(const std::string& username, const std::string& avatar_url);

    // 8. 昵称修改
    bool setNickname(const std::string& username, const std::string& nickname);

    // 9. 签名修改
    bool setSignature(const std::string& username, const std::string& signature);

    // 10. 手机号修改
    bool setPhoneNumber(const std::string& username, const std::string& phone_number);
};
