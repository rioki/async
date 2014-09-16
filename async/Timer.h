
#ifndef _ASYNC_TIMER_H_
#define _ASYNC_TIMER_H_

#include <memory>
#include <functional>
#include <ctime>
#include <uv.h>

namespace async
{        
    class EventLoop;
    
    class Timer 
    {
    public:
        
        Timer(uv_loop_t* loop, std::function<void ()> cb);
        
        ~Timer();
        
        void start(time_t timeout, time_t repeat);
        
        void stop();
        
    private:
        uv_timer_t             handle;
        std::function<void ()> cb;
        std::shared_ptr<Timer> self;
                
        Timer(const Timer&) = delete;
        const Timer& operator = (const Timer&) = delete;
        
        friend void on_timer(uv_timer_t* handle);
        friend std::shared_ptr<Timer> set_timeout(time_t timeout, std::function<void ()> cb);
        friend std::shared_ptr<Timer> set_timeout(EventLoop& loop, time_t timeout, std::function<void ()> cb);
        friend std::shared_ptr<Timer> set_interval(time_t interval, std::function<void ()> cb);    
        friend std::shared_ptr<Timer> set_interval(EventLoop& loop, time_t interval, std::function<void ()> cb);
    };
    
    std::shared_ptr<Timer> set_timeout(time_t timeout, std::function<void ()> cb);
    
    std::shared_ptr<Timer> set_timeout(EventLoop& loop, time_t timeout, std::function<void ()> cb);
    
    std::shared_ptr<Timer> set_interval(time_t interval, std::function<void ()> cb);
    
    std::shared_ptr<Timer> set_interval(EventLoop& loop, time_t interval, std::function<void ()> cb);
}

#endif
