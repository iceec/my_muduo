#pragma once
#include <thread>
#include <memory>
#include <functional>
#include <string>
#include <atomic>



// 类似于代理模式 延迟绑定 start的时候才会打开
// 观点就是可不可返回值有类型 看看后面可不可以改改
// thread可以自己确定自己的tid 在start 中有保证
class Thread
{
public:
    using Thread_func = std::function<void()>;

    void start();
    void join();

    explicit Thread(Thread_func, std::string_view default_name="");
    ~Thread();

    bool started() { return _started; }
    bool joined() { return _joined; }

    pid_t tid() const { return _tid; }

    std::string_view thread_name() { return _thread_name; }

    static int thread_cnt() { return _thread_cnt; }

private:
    std::string default_thread_name();
    std::shared_ptr<std::thread> _thread; // 为什么用sharene
    bool _joined{false};
    bool _started{false};
    pid_t _tid;        // thread_id
    Thread_func _func; // 执行的函数 里面有loop的
    std::string _thread_name;
    static std::atomic_int32_t _thread_cnt;
};