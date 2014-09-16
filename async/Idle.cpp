
#include "Idle.h"

#include "EventLoop.h"

namespace async
{
    Idle::Idle(uv_loop_t* loop, std::function<void ()> c)
    : cb(c)
    {
        handle.data = this;
        int r = uv_idle_init(loop, &handle);
        if (r < 0) 
        {
            throw std::runtime_error(uv_strerror(r));
        }
    }
    
    Idle::~Idle() {}
        
    void on_idle(uv_idle_t* handle)
    {
        Idle* idle = (Idle*)handle->data;
        if (idle->cb)
        {
            idle->cb();
        }        
    }
        
    void Idle::start()
    {
        int r = uv_idle_start(&handle, &on_idle);
        if (r < 0) 
        {
            throw std::runtime_error(uv_strerror(r));
        }
    }
    
    void Idle::stop()
    {
        int r = uv_idle_stop(&handle);
        if (r < 0) 
        {
            throw std::runtime_error(uv_strerror(r));
        }
        self.reset();
    }
    
    std::shared_ptr<Idle> set_idle(std::function<void ()> cb)
    {
        auto idle = std::make_shared<Idle>(uv_default_loop(), cb);
        idle->self = idle;        
        idle->start();        
        return idle;
    }
    
    std::shared_ptr<Idle> set_idle(EventLoop& loop, std::function<void ()> cb)
    {
        auto idle = std::make_shared<Idle>(loop.c_obj(), cb);
        idle->self = idle;        
        idle->start();        
        return idle;
    }
}
