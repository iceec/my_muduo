#include <Thread.h>
#include <semaphore.h>
#include <CurrentThread.h>
#include<assert.h>
std::atomic_int32_t Thread::_thread_cnt{0};
void Thread::start()
{
    if(_started)
        return;

    sem_t sem;
    sem_init(&sem, false, 0);

    auto func = [&]()
    {
        _tid = CurrentThread::gettid();
        // 先通知
        sem_post(&sem);
        _func();
    };
    _thread = std::make_shared<std::thread>(func);

    sem_wait(&sem);
    // 保证了 sem 不会提前消灭 并且还能保证 tid OK了
    _started = true;
}
void Thread::join()
{
    if (_joined)
        return;

    _joined = true;
    _thread->join();
}

Thread::Thread(Thread_func func, std::string_view default_name) : _func(func)
{
    if (default_name.size() > 0)
        _thread_name = default_name;
    else
        _thread_name = default_thread_name();

    ++_thread_cnt;  // 并非表示开启的线程
}

std::string Thread::default_thread_name()
{
    std::string result = "thread ";
    result += std::to_string(_thread_cnt);
    return result;
}

Thread::~Thread()
{
    // 已经开始了 但没join
    if(_started && !_joined)
    {
        _thread->detach(); // 那就分离把 不阻塞
    }
    assert(_thread_cnt > 0);
    --_thread_cnt;
    
}