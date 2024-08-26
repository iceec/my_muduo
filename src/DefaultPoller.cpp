#include "Poller.h"
 #include"EPoller.h"
Poller *
Poller::new_default_poller(EventLoop * loop)
{
    #ifdef USE_POLL
        return nullptr;
    #endif
        return new EPoller(loop);
}