
#ifndef _ASYNC_IP_ADDRESS_H_
#define _ASYNC_IP_ADDRESS_H_

#include <iosfwd>
#include <functional>
#include <uv.h>

namespace async
{
    class EventLoop;

    class IpAddress
    {
    public:
        IpAddress();
        
        IpAddress(unsigned int host, unsigned short port);
        
        IpAddress(const IpAddress& other);
        
        ~IpAddress();
        
        const IpAddress& operator = (const IpAddress& other);
        
        bool operator == (const IpAddress& other) const;
        
        bool operator != (const IpAddress& other) const;
        
        unsigned int get_host() const;
        
        unsigned short get_port() const;
        
        operator sockaddr* ();
        
        operator const sockaddr* () const;
        
    private:    
        sockaddr_in addr;
    };
    
    std::ostream& operator << (std::ostream& os, const IpAddress& addr);    
    
    void dns_resolve(const std::string& host, unsigned short port, std::function<void (IpAddress)> cb);
    void dns_resolve(EventLoop& loop, const std::string& host, unsigned short port, std::function<void (IpAddress)> cb);
}

#endif
