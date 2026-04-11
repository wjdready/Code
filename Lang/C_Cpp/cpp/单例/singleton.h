#pragma once

#include <memory>
#include <mutex>

template <typename T> class Singleton
{
  public:
    // 获取单例实例
    static T &getInstance();

    // 禁用拷贝构造和赋值运算符
    Singleton(const Singleton &other) = delete;
    Singleton<T> &operator=(const Singleton &other) = delete;

  private:
    // 静态互斥锁，保证线程安全
    static std::mutex mutex;
    // 静态智能指针，管理单例对象
    static std::unique_ptr<T> instance;

    // 私有构造函数，防止外部实例化
    Singleton() = default;
    ~Singleton() = default;
};

// 静态成员初始化
template <typename T> std::mutex Singleton<T>::mutex;
template <typename T> std::unique_ptr<T> Singleton<T>::instance;

template <typename T> T &Singleton<T>::getInstance()
{
    // 双重检查锁（DCLP），减少锁竞争
    if (!instance)
    { // 等价于原代码的isNull()
        // 加锁保证线程安全
        std::lock_guard<std::mutex> lock(mutex); // 自动解锁，避免手动unlock的遗漏
        if (!instance)
        {
            // 创建实例并交由unique_ptr管理
            instance.reset(new T());
        }
    }
    return *instance;
}

/*
 * 单例宏定义，使用方式与原代码一致
 * 功能：
 * 1. 将类的构造/析构/拷贝构造/赋值运算符设为私有
 * 2. 提供静态的getInstance方法
 * 3. 将Singleton设为友元，使其能访问私有构造函数
 */
#define SINGLETON(Class)                                                                                               \
  private:                                                                                                             \
    Class();                                                                                                           \
    ~Class();                                                                                                          \
    Class(const Class &other);                                                                                         \
    Class &operator=(const Class &other);                                                                              \
                                                                                                                       \
  public:                                                                                                              \
    static Class &getInstance()                                                                                        \
    {                                                                                                                  \
        return Singleton<Class>::getInstance();                                                                        \
    };                                                                                                                 \
    friend class Singleton<Class>;                                                                                     \
    friend struct std::default_delete<Class>;
