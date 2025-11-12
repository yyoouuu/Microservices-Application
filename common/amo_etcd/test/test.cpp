#include "amo_etcd.h"

int main()
{
    //服务发现
    std::string etcd_host = "http://127.0.0.1:2379";
    std::string basedir = "server/user";
    
    auto put_cb = [](const std::string &key, const std::string &value)
    {
        //可以增加服务上线的本地缓存功能
    };

    auto del_cb = [](const std::string &key, const std::string &value)
    {
        //可以增加服务下线的本地缓存功能
    };

    etcd_ns::Discovery::s_ptr discoverer = std::make_shared<etcd_ns::Discovery>(etcd_host,basedir,put_cb,del_cb);

    //模拟注册服务
    etcd_ns::Registry::s_ptr _register = std::make_shared<etcd_ns::Registry>(etcd_host,3);
    std::string key = basedir + "/instance2";
    std::string value = "127.0.0.1:8081";
    std::cout << "模拟注册服务..." << std::endl;
    auto resp1 = _register->registry(basedir + "/instance1","127.0.0.1:8080");
    auto resp2 = _register->registry(key,value);
    std::cout << "模拟注销服务..." << std::endl;
    _register->unregistry(basedir + "/instance1");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}