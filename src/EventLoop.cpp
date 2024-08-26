#include <EventLoop.h>
#include <sys/eventfd.h>
#include <Log.hpp>
#include <Poller.h>
#include <CurrentThread.h>
#include <Channel.h>
// 是在TLS数据区 但是我运行起来看 好像就是在自己的栈上啊 可能自己的栈就是TLS
__thread EventLoop *t_this_thread_event_loop = nullptr;

constexpr int kPollTimeMs = 10000; // 10s

int create_event_fd()
{
    int eventfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);

    if (eventfd < 0)
    {
        LOG_FATAL("event_fd is neg\n");
    }
    return eventfd;
}

EventLoop::EventLoop() : _thread_id(CurrentThread::gettid()),
                         _poller(Poller::new_default_poller(this)),
                         _wake_up_fd(create_event_fd()),
                         _wake_up_channel(new Channel(this, _wake_up_fd))
{
    // 这个线程创建过一个loop 了
    if (t_this_thread_event_loop)
    {
        LOG_FATAL("%d thread has another loop %p/n", _thread_id, this);
    }

    // 设置当前线程的loop
    t_this_thread_event_loop = this;

    // 设置loop 的read函数以及 使能
    _wake_up_channel->set_read_call_back(std::bind(&EventLoop::wake_up_handle_read, this));
    _wake_up_channel->enable_read();
}

EventLoop::~EventLoop()
{
    // 移除wake up
    _wake_up_channel->disable_all();
    _wake_up_channel->remove();
    ::close(_wake_up_fd);
    // 消除loop
    t_this_thread_event_loop = nullptr;
}
// ? 这里的唤醒不用条件变量而是用一个文件描述符 说是效率更高 ？？
// epollfd 实际是在内存 64为的计数  write+值  read会全部读出来
// epollfd  可以用来实现文件的异步 进程之间 线程之间的通信
void EventLoop::wake_up_handle_read()
{
    uint64_t one = 1;

    auto size = ::read(_wake_up_fd, &one, sizeof(one));

    if (size != sizeof(one))
    {
        LOG_WARNING("EventLoop::handle_read() read %lu instead of 8", size);
    }
}

void EventLoop::wake_up()
{
    uint64_t one = 1;

    auto size = ::write(_wake_up_fd, &one, sizeof(one));

    if (size != sizeof(one))
    {
        LOG_WARNING("EventLoop::wakeup() write %lu instead of 8", size);
    }
}

void EventLoop::update_channel(Channel *channel)
{
    _poller->update_channel(channel);
}
void EventLoop::remove_channel(Channel *channel)
{
    _poller->remove_channel(channel);
}
bool EventLoop::has_channel(Channel *channel)
{
    return _poller->has_channel(channel);
}

void EventLoop::loop()
{

    _quit = false;
    _looping = true;

    // 别人可能置为
    while (!_quit)
    {
        _poller_return_time = _poller->poll(kPollTimeMs, &_active_channel);

        for (Channel *channel : _active_channel)
        {
            // 每一个文件描述符的回调函数
            channel->handle_event(_poller_return_time);
        }

        // 给这个线程的专属回调函数 可能是主rect给到一个新的channel得去添加了 
        doPendingFunctors();
    }
    // quit = true 了
    _looping = false;
}

void EventLoop::quit()
{
    // 可能是不同线程 但是随意了
    _quit = true;

    // 要是loop 没在自己的线程  唤醒 自己的线程  
    if (!is_in_loop_thread())
    {
        wake_up();
    }
}

void EventLoop::run_in_loop(Functor func)
{

    if (is_in_loop_thread())
    {
        func();
    }
    else
    {
        queue_in_loop(func);
    }
}


void EventLoop::queue_in_loop(Functor func)
{
    // 进来表明是不同线程的操作
    {
        std::lock_guard<std::mutex> tmp(_pending_lock);
        _pending_functor.emplace_back(func);
    }

    // 表明不只是 run_in_loop 调用可能自己调用  ？？？
    if (!is_in_loop_thread() || _calling_prending_functor)
    {
        wake_up();
    }
}

void EventLoop::doPendingFunctors()
{
    _calling_prending_functor = true;
    std::vector<Functor> func_vec;

    // 非常巧妙的设计 只需要交换指针就行了
    {
        std::lock_guard<std::mutex> tmp(_pending_lock);
        func_vec.swap(_pending_functor);
    }


    for(auto & func : func_vec)
    {
        func();
    }

    _calling_prending_functor = false;

}