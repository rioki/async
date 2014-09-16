
#include "Timer.h"

#include <uv.h>
#include <stdexcept>

#include "EventLoop.h"

namespace async
{
    Timer::Timer(uv_loop_t* loop, std::function<void ()> c)
    : cb(c)
    {
        handle.data = this;
        int r = uv_timer_init(loop, &handle);
        if (r < 0) 
        {
            throw std::runtime_error(uv_strerror(r));
        }
    }
    
    Timer::~Timer() {}
    
    void on_timer(uv_timer_t* handle)
    {
        Timer* timer = (Timer*)handle->data;
        if (timer->cb)
        {
            timer->cb();
        }
        if (uv_timer_get_repeat(handle) == 0)
        {
            timer->self.reset();
        }
    }
    
    void Timer::start(time_t timeout, time_t repeat)
    {
        int r = uv_timer_start(&handle, &on_timer, timeout, repeat);
        if (r < 0) 
        {
            throw std::runtime_error(uv_strerror(r));
        }
    }
    
    void Timer::stop() 
    {
        int r = uv_timer_stop(&handle);
        if (r < 0) 
        {
            throw std::runtime_error(uv_strerror(r));
        }
        self.reset();
    }

    std::shared_ptr<Timer> set_timeout(time_t timeout, std::function<void ()> cb)
    {
        auto timer = std::make_shared<Timer>(uv_default_loop(), cb);
        timer->self = timer;        
        timer->start(timeout, 0);        
        return timer;
    }
    
    std::shared_ptr<Timer> set_timeout(EventLoop& loop, time_t timeout, std::function<void ()> cb)
    {
        auto timer = std::make_shared<Timer>(loop.c_obj(), cb);
        timer->self = timer;        
        timer->start(timeout, 0);        
        return timer;
    }
    
    std::shared_ptr<Timer> set_interval(time_t interval, std::function<void ()> cb)
    {
        auto timer = std::make_shared<Timer>(uv_default_loop(), cb);
        timer->self = timer;        
        timer->start(interval, interval);        
        return timer;
    }
    
    std::shared_ptr<Timer> set_interval(EventLoop& loop, time_t interval, std::function<void ()> cb)
    {
        auto timer = std::make_shared<Timer>(loop.c_obj(), cb);
        timer->self = timer;        
        timer->start(interval, interval);        
        return timer;
    }
}
