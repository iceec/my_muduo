#include "CurrentThread.h"

void CurrentThread::cache_tid()
{
    t_cache_tid = static_cast<pid_t>(::syscall(SYS_gettid)); 
}