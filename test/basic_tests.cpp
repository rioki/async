
#include <rtest.h>

#include <async.h>
#include <ctime>

SUITE(basic)
{
    TEST(run)
    {
        async::run();
    }
    
    TEST(run_event_loop)
    {
        async::EventLoop loop;
        loop.run();
    }
    
    TEST(timeout)
    {
        clock_t start = std::clock();
        clock_t end   = 0;
        async::set_timeout(500, [&] () {
            end = std::clock();
        });        
        async::run();
        
        float diff = (float)(end - start) / (float)CLOCKS_PER_SEC;
        CHECK(diff > 0.49);
        CHECK(diff < 0.51);
    }
    
    TEST(timeout_event_loop)
    {
        async::EventLoop loop;
        
        clock_t start = std::clock();
        clock_t end   = 0;
        async::set_timeout(loop, 500, [&] () {
            end = std::clock();
        });        
        loop.run();
        
        float diff = (float)(end - start) / (float)CLOCKS_PER_SEC;
        CHECK(diff > 0.49);
        CHECK(diff < 0.51);
    }
    
    TEST(cancel_timeout)
    {
        clock_t start = std::clock();
        clock_t end   = 0;
        auto t = async::set_timeout(500, [&] () {
            end = std::clock();
        });        
        
        t->stop();        
        async::run();
        
        CHECK(end == 0);
    }
    
    TEST(invervall)
    {
        clock_t start = std::clock();
        clock_t end   = 0;
        unsigned int count = 0;
        std::shared_ptr<async::Timer> i = async::set_interval(100, [&] () {
            end = std::clock();
            count++;
            if (count == 5)
            {
                i->stop();
            }
        });        
        async::run();
        
        float diff = (float)(end - start) / (float)CLOCKS_PER_SEC;
        CHECK(diff > 0.49);
        CHECK(diff < 0.51);
        CHECK(count == 5);
    }
    
    TEST(idle)
    {
        unsigned int count = 0;
        std::shared_ptr<async::Idle> i = async::set_idle([&] () {
            count++;
            if (count == 5)
            {
                i->stop();
            }
        });        
        async::run();
        
        CHECK(count == 5);
    }
}
