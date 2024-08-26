#include "CurrentThread.h"
__thread int CurrentThread::t_cache_tid = 0;
void CurrentThread::cache_tid()
{
    t_cache_tid = static_cast<pid_t>(::syscall(SYS_gettid)); 
}