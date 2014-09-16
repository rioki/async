
#include "EventLoop.h"

#include <uv.h>
#include <stdexcept>

namespace async
{  
    EventLoop::EventLoop()
    {
        int r = uv_loop_init(&obj);
        if (r < 0) 
        {
            throw std::runtime_error(uv_strerror(r));
        }
    }
        
    EventLoop::~EventLoop()
    {
        int r = uv_loop_close(&obj);
        // and now?
    }
        
    void EventLoop::run()
    {
        int r = uv_run(&obj, UV_RUN_DEFAULT);
        if (r < 0) 
        {
            throw std::runtime_error(uv_strerror(r));
        }
    }
    
    void EventLoop::run_once()
    {
        int r = uv_run(&obj, UV_RUN_NOWAIT);
        if (r < 0) 
        {
            throw std::runtime_error(uv_strerror(r));
        }
    }
    
    void EventLoop::stop()
    {
        uv_stop(&obj);
    }
    
    uv_loop_t* EventLoop::c_obj()
    {
        return &obj;
    }

    void run()
    {
        int r = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
        if (r < 0) 
        {
            throw std::runtime_error(uv_strerror(r));
        }
    }
        
    void run_once()
    {
        int r = uv_run(uv_default_loop(), UV_RUN_NOWAIT);
        if (r < 0) 
        {
            throw std::runtime_error(uv_strerror(r));
        }
    }
        
    void stop()
    {
        uv_stop(uv_default_loop());
    }
}
