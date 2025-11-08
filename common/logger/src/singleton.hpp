#pragma once
#include <iostream>
#include <mutex>

// 普通单例模板
template<typename T>
class Singleton {
public:
    // 获取单例实例的函数
    static T* instance() {
        if (instance_ == nullptr) {
            std::unique_lock<std::mutex> lck(mutex_);
            if (instance_ == nullptr) {
                instance_ = new T();
                static Garbor gar;
            }
        }
        return instance_;
    }

    // 手动控制实例的锁
    void lock() {
        mutex_self_.lock();
    }

    void unlock() {
        mutex_self_.unlock();
    }

protected:
    // 受保护的构造函数和虚析构函数，以强制实现单例模式
    explicit Singleton() {}
    virtual ~Singleton() {}

    // 禁用复制构造函数和赋值运算符，以防止克隆
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    // 垃圾回收器类，用于自动释放单例实例
    class Garbor {
    public:
        Garbor() {}
        ~Garbor() {
            if (instance_) {
                delete instance_;
                instance_ = nullptr;
            }
        }
    };

protected:
    // 指向单例实例的静态指针
    static T* instance_;
    // 用于确保在实例创建时线程安全的静态互斥锁
    static std::mutex mutex_;
    // 自定义锁机制的静态互斥锁
    static std::mutex mutex_self_;
};

// 静态成员初始化
template<typename T>
T* Singleton<T>::instance_ = nullptr;

template<typename T>
std::mutex Singleton<T>::mutex_;

template<typename T>
std::mutex Singleton<T>::mutex_self_;