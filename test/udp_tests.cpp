
#include <rtest.h>

#include <async.h>
#include <ctime>

SUITE(udp)
{
    TEST(dns_resolve)
    {
        async::IpAddress result;
        async::dns_resolve("test.com", 1337, [&] (async::IpAddress addr) {
            result = addr;
        });
        
        async::run();
        
        CHECK_EQUAL(async::IpAddress(0x3217FF31, 1337), result);
    }
    
    /*TEST(echo)
    {
        // simple UDP echo
        std::shared_ptr<UdpSocket> server = async::create_udp_socket([&] (async::IpAddress addr, std::vector<char> data) {
            server->send(addr, data);
        });
        server->bind(1337);
        
        std::string resp;
        auto client = async::create_udp_socket([&] (async::IpAddress addr, std::vector<char> data) {
            resp = std::string(data.begin(), data.end());
            async::stop();
        });
        
        async::dns_resolve("localhost", 1337, [&] (async::IpAddress& addr) {
            client.send(addr, "Hello UDP!");
        });
        
        async::run();
        
        CHECK_EQUAL("Hello UDP!", resp);
    }*/
}

