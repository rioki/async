
#include "IpAddress.h"

#include <iostream>
#include <cstring>

#include "EventLoop.h"

namespace async
{
    IpAddress::IpAddress()
    {
        std::memset(&addr, 0, sizeof(sockaddr_in));
    }
    
    IpAddress::IpAddress(unsigned int host, unsigned short port)
    {
        std::memset(&addr, 0, sizeof(sockaddr_in));
        addr.sin_addr.s_addr = htonl(host);
        addr.sin_port        = htons(port);
    }
    
    IpAddress::IpAddress(const IpAddress& other)
    {
        std::memcpy(&addr, &other.addr, sizeof(sockaddr_in));
    }
        
    IpAddress::~IpAddress() {}
        
    const IpAddress& IpAddress::operator = (const IpAddress& other)
    {
        if (this != &other)
        {
            std::memcpy(&addr, &other.addr, sizeof(sockaddr_in));
        }
        return *this;
    }
    
    bool IpAddress::operator == (const IpAddress& other) const
    {
        return get_host() == other.get_host() &&
               get_port() == other.get_port();
    }
        
    bool IpAddress::operator != (const IpAddress& other) const
    {
        return !(*this == other);
    }
    
    unsigned int IpAddress::get_host() const
    {
        return ntohl(addr.sin_addr.s_addr);
    }
        
    unsigned short IpAddress::get_port() const
    {
        return ntohs(addr.sin_port);
    }
    
    IpAddress::operator sockaddr* ()
    {
        return reinterpret_cast<sockaddr*>(&addr);
    }
    
    IpAddress::operator const sockaddr* () const
    {
        return reinterpret_cast<const sockaddr*>(&addr);
    }
    
    std::ostream& operator << (std::ostream& os, const IpAddress& addr)
    {
        unsigned int host = addr.get_host();
        unsigned short port = addr.get_port();
        
        unsigned int a = (host & 0xFF000000) >> 24;
        unsigned int b = (host & 0x00FF0000) >> 16;
        unsigned int c = (host & 0x0000FF00) >>  8;
        unsigned int d = (host & 0x000000FF) >>  0;
        
        os << a << "." << b << "." << c << "." << d << ":" << port;
        
        return os;
    }
    
    struct DnsResolve
    {
        uv_getaddrinfo_t req;
        std::function<void (IpAddress)> cb;
        unsigned short port;
    };
    
    void on_dns_resolve(uv_getaddrinfo_t* req, int status, addrinfo* res)
    {
        DnsResolve* baton = (DnsResolve*)req->data;
                
        if (baton->cb)
        {
            bool found = false;
            while (res != NULL)
            {
                if (res->ai_family == AF_INET)
                {
                    unsigned int host = ((sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
                    baton->cb(IpAddress(ntohl(host), baton->port));
                    found = true;
                    break;
                }
                
                res = res->ai_next;
            }
            
            if (found == false)
            {
                // fallback... maybe some diagnostic?
                baton->cb(IpAddress()); 
            }
        }
        
        delete baton;
    }
    
    void dns_resolve(const std::string& host, unsigned short port, std::function<void (IpAddress)> cb)
    {
        DnsResolve* baton = new DnsResolve;
        baton->req.data = baton;
        baton->cb       = cb;
        baton->port     = port;
        
        int r = uv_getaddrinfo(uv_default_loop(), &baton->req, on_dns_resolve, host.c_str(), NULL, NULL);
        if (r < 0) 
        {
            throw std::runtime_error(uv_strerror(r));
        }
    }
    
    void dns_resolve(EventLoop& loop, const std::string& host, unsigned short port, std::function<void (IpAddress)> cb)
    {
        DnsResolve* baton = new DnsResolve;
        baton->req.data = baton;
        baton->cb       = cb;
        baton->port     = port;
        
        int r = uv_getaddrinfo(loop.c_obj(), &baton->req, on_dns_resolve, host.c_str(), NULL, NULL);
        if (r < 0) 
        {
            throw std::runtime_error(uv_strerror(r));
        }
    }
}
