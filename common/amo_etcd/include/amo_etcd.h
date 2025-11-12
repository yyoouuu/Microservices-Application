#pragma once
#include "etcd/Client.hpp"
#include "etcd/KeepAlive.hpp"
#include "etcd/Watcher.hpp"
#include "etcd/Response.hpp"
#include <thread>
#include <atomic>
#include <memory>
#include <functional>
#include <unordered_map>
#include "logger.h"

namespace etcd_ns
{
    class Registry
    {
    private:
        std::shared_ptr<etcd::Client> _client;
        std::shared_ptr<etcd::KeepAlive> _keepalive;
        uint64_t _lease_id{0};
        std::atomic<bool> _running;
        std::unordered_map<std::string, std::string> _keys;

    public:
        using s_ptr = std::shared_ptr<Registry>;

        Registry(const std::string &host, const int &lease_ttl)
            : _client(std::make_shared<etcd::Client>(host)), _keepalive(_client->leasekeepalive(lease_ttl).get()), _lease_id(_keepalive->Lease()), _running(true)
        {
        }

        ~Registry()
        {
            _running = false;
        }

        // 注册
        bool registry(const std::string &key, const std::string &val)
        {
            if (_lease_id == 0)
            {
                APP_ERROR("Lease 尚未创建，无法注册");
                return false;
            }

            try
            {
                auto resp = _client->put(key, val, _lease_id).get();
                if (!resp.is_ok())
                {
                    APP_ERROR("注册失败: {}", resp.error_message());
                    return false;
                }
                APP_INFO("注册成功: {}   {}",key, val);
                _keys[key] = val;
                return true;
            }
            catch (const std::exception &e)
            {
                APP_ERROR("注册异常: {}", e.what());
                return false;
            }
        }

        // 注销服务
        void unregistry(const std::string &key)
        {
            try
            {
                _client->rm(key).get();
                _keys.erase(key);
                APP_DEBUG("服务注销: {}", key);
            }
            catch (const std::exception &e)
            {
                APP_ERROR("注销异常: {}", e.what());
            }
        }
    };

    // 服务发现
    class Discovery
    {

    public:
        using s_ptr = std::shared_ptr<Discovery>;
        using NotifyCallback = std::function<void(const std::string &, const std::string &)>;

        Discovery(const std::string &host,
                  const std::string &basedir,
                  NotifyCallback put_cb,
                  NotifyCallback del_cb)
            : _client(std::make_shared<etcd::Client>(host)),
              _basedir(basedir),
              _put_cb(std::move(put_cb)),
              _del_cb(std::move(del_cb))
        {

            auto resp = _client->ls(_basedir).get();
            if (!resp.is_ok())
            {
                APP_ERROR("获取服务数据失败: {}", resp.error_message());
                return;
            }
            for (int i = 0; i < resp.keys().size(); ++i)
            {
                if (_put_cb)
                    _put_cb(resp.key(i), resp.value(i).as_string());
            }

            // 监控服务变化
            _watcher = std::make_shared<etcd::Watcher>(*_client, _basedir, [this](const etcd::Response &resp)
                                                       { this->callback(resp); }, true);
        }

    private:
        void callback(const etcd::Response &resp)
        {
            if (!resp.is_ok())
            {
                APP_ERROR("事件通知错误: {}", resp.error_message());
                return;
            }

            for (const auto &ev : resp.events())
            {
                try
                {
                    if (ev.event_type() == etcd::Event::EventType::PUT)
                    {
                        if (_put_cb)
                            _put_cb(ev.kv().key(), ev.kv().as_string());
                        APP_DEBUG("新增服务: {}-{}", ev.kv().key(), ev.kv().as_string());
                    }
                    else if (ev.event_type() == etcd::Event::EventType::DELETE_)
                    {
                        if (_del_cb)
                            _del_cb(ev.prev_kv().key(), ev.prev_kv().as_string());
                        APP_DEBUG("下线服务: {}-{}", ev.prev_kv().key(), ev.prev_kv().as_string());
                    }
                }
                catch (const std::exception &e)
                {
                    APP_ERROR("事件处理异常: {}", e.what());
                }
            }
        }
            private:
        std::shared_ptr<etcd::Client> _client;
        std::shared_ptr<etcd::Watcher> _watcher;
        std::string _basedir;
        NotifyCallback _put_cb;
        NotifyCallback _del_cb;
    };
}