#pragma once
#include <unistd.h>
#include <sys/syscall.h>

namespace CurrentThread
{

    extern __thread int t_cache_tid;

    void cache_tid();


    // 注意每一个线程的应该不同 
    inline int gettid()
    {
        if(t_cache_tid == 0)
            cache_tid();
        return t_cache_tid;
    }





}