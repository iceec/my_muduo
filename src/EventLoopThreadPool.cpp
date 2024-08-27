#include<EventLoopThreadPool.h>
#include<EventLoopThread.h>
EventLoop * EventLoopThreadPool::get_next_loop()
{
    EventLoop * result = _base_loop;

    if(_thread_pool.size() > 0 )
    {
        result = _event_loop_pool[_next_idx];

        _next_idx = (_next_idx + 1 ) % _event_loop_pool.size();
    }
    return result;
}

std::vector<EventLoop *> EventLoopThreadPool::get_all_loop()
{
    if(_event_loop_pool.size() == 0)
        return {_base_loop};
    
    return _event_loop_pool;
}

void EventLoopThreadPool::start(CallBack func)
{
    _started = true;

    for(int i=0;i<_thread_cnt;++i)
    {
        _thread_pool.emplace_back(std::make_unique<EventLoopThread>(func,_thread_pool_name +std::to_string(i+1)));

        _event_loop_pool.emplace_back(_thread_pool.back()->start_loop());
    }

    if(_thread_cnt == 0 && func)
    {
        func(_base_loop); // 总要初始化 工作线程的
    }
}