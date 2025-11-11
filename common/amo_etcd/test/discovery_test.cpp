#include "amo_etcd.h"

int main()
{

    //服务注册
    std::string etcd_host = "http://127.0.0.1:2379";
    std::string basedir = "server/user/instance1";
    
    auto put_cb = [](const std::string &key, const std::string &value)
    {
        APP_INFO("服务上线[PUT]: {}   {}",key, value);
    };

    auto del_cb = [](const std::string &key, const std::string &value)
    {
        APP_INFO("服务下线[DELETE]: {}   {}",key, value);
    };

    grpc_ns::Discovery::s_ptr discoverer = std::make_shared<grpc_ns::Discovery>(etcd_host,basedir,put_cb,del_cb);

    //模拟注册服务
    grpc_ns::Registry::s_ptr _register = std::make_shared<grpc_ns::Registry>(etcd_host,3);
    std::string key = basedir+"instance1";
    std::string value = "127.0.0.1:8080";
    std::cout << "模拟注册服务..." << std::endl;
    auto resp = _register->registry(key,value);
    if (resp)
    {
        std::cout << "注册成功: " << key << " -> " << value << std::endl;
    }
    else
    {
        std::cout << "注册失败: " << key << std::endl;
    }
    std::cout << "模拟注册服务..." << std::endl;
    _register->unregistry(key);


    std::this_thread::sleep_for(std::chrono::seconds(2));



    return 0;
}