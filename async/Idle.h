
#ifndef _ASYNC_IDLE_H_
#define _ASYNC_IDLE_H_

#include <memory>
#include <functional>
#include <uv.h>

namespace async
{
    class EventLoop;

    class Idle
    {
    public:
        
        Idle(uv_loop_t* loop, std::function<void ()> cb);
        
        ~Idle();
        
        void start();
        
        void stop();
        
    private:
        uv_idle_t handle;
        std::function<void ()> cb;
        std::shared_ptr<Idle> self;
        
        Idle(const Idle&) = delete;
        const Idle& operator = (const Idle&) = delete;
        
        friend void on_idle(uv_idle_t* handle);
        friend std::shared_ptr<Idle> set_idle(std::function<void ()> cb);
        friend std::shared_ptr<Idle> set_idle(EventLoop& loop, std::function<void ()> cb);
    };
    
    std::shared_ptr<Idle> set_idle(std::function<void ()> cb);
    
    std::shared_ptr<Idle> set_idle(EventLoop& loop, std::function<void ()> cb);
}

#endif
