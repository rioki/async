
#ifndef _ASYNC_EVENT_LOOP_H_
#define _ASYNC_EVENT_LOOP_H_

#include <uv.h>

namespace async
{
    /**
     * Event Loop
     **/
    class EventLoop
    {
    public:
        EventLoop();
        
        ~EventLoop();
        
        void run();
        
        void run_once();
        
        void stop();
        
        uv_loop_t* c_obj();
        
    private:
        uv_loop_t obj;
        
        EventLoop(const EventLoop&) = delete;
        const EventLoop& operator = (const EventLoop&) = delete;
    };
    
    /**
     * Run the default event loop.
     **/
    void run();
    
    /**
     * Run once the default loop, without waiting on I/O
     **/
    void run_once();
    
    /**
     * Stop the main loop from executing.
     **/
    void stop();
}

#endif
