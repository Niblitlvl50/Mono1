//
//  SysNetwork.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 22/01/16.
//
//

#include "SysNetwork.h"
#include <stdexcept>
#include <cstdio>

#define ZED_NET_IMPLEMENTATION
#define ZED_NET_STATIC

#include "zed_net/zed_net.h"
#include <ifaddrs.h>
#include <net/if.h>

namespace
{
    class UDPSocket : public Network::ISocket
    {
    public:

        UDPSocket(const char* address, int port, bool blocking)
        {
            const int address_result = zed_net_get_address(&m_address, address, port);
            if(address_result != 0)
                throw std::runtime_error("Unable to resolve host address");

            m_handle = zed_net_udp_socket_open(port, !blocking);
            if(!m_handle)
                throw std::runtime_error("Failed to create socket");

            constexpr int yes = 1;
            const int result1 = setsockopt(m_handle->handle, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
            if(result1 == -1)
                throw std::runtime_error("Unable to set reuse address option");

            const int result2 = setsockopt(m_handle->handle, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));
            if(result2 == -1)
                throw std::runtime_error("Unable to set broadcast option");
        }

        ~UDPSocket()
        {
            zed_net_udp_socket_close(m_handle);
        }

        int Send(const std::vector<byte>& data)
        {
            const int size = static_cast<int>(data.size());
            return zed_net_udp_socket_send(m_handle, m_address, data.data(), size);
        }

        int SendTo(const std::vector<byte>& data, const char* address, int port)
        {
            const int size = static_cast<int>(data.size());

            zed_net_address_t socket_address;
            const int address_result = zed_net_get_address(&socket_address, address, port);
            if(address_result != 0)
                return -1;

            return zed_net_udp_socket_send(m_handle, socket_address, data.data(), size);
        }

        bool Receive(std::vector<byte>& data)
        {
            zed_net_address_t sender;
            const int size = static_cast<int>(data.capacity());
            return zed_net_udp_socket_receive(m_handle, &sender, data.data(), size) != 0;
        }

        zed_net_address_t m_address;
        zed_net_udp_socket_t* m_handle;
    };

    std::string GetBroadcastAddress()
    {
        std::string broadcast_string;

        ifaddrs* devices = nullptr;
        const int result = getifaddrs(&devices);
        if(result != 0)
            return broadcast_string;

        for(ifaddrs* it = devices; it != nullptr; it = it->ifa_next)
        {
            if(!it->ifa_addr || it->ifa_addr->sa_family != AF_INET)
                continue;

            const bool loopback = (it->ifa_flags & IFF_LOOPBACK);
            if(loopback)
                continue;

            const sockaddr_in* broadcast_addr = reinterpret_cast<const sockaddr_in*>(it->ifa_broadaddr);
            broadcast_string = inet_ntoa(broadcast_addr->sin_addr);
            break;
        }

        freeifaddrs(devices);

        return broadcast_string;
    }
}


void Network::Init()
{
    const int result = zed_net_init();
    if(result != 0)
        throw std::runtime_error("Unable to initialize network");
}

void Network::Exit()
{
    zed_net_shutdown();
}

std::string Network::GetLocalHostName()
{
    constexpr int buffer_size = 128;
    char buffer[buffer_size];
    gethostname(buffer, buffer_size);

    return buffer;
}

std::shared_ptr<Network::ISocket> Network::CreateUDPSocket(int port, bool blocking)
{
    return OpenUDPSocket(0, port, blocking);
}

std::shared_ptr<Network::ISocket> Network::OpenUDPSocket(const char* address, int port, bool blocking)
{
    try
    {
       return std::make_shared<UDPSocket>(address, port, blocking);
    }
    catch(const std::runtime_error& error)
    {
        std::printf("%s\n", error.what());
        std::printf("zed_net_error: %s", zed_net_get_error());
    }

    return nullptr;
}

std::shared_ptr<Network::ISocket> Network::OpenBroadcastSocket(int port, bool blocking)
{
    const std::string& broadcast_string = GetBroadcastAddress();
    return OpenUDPSocket(broadcast_string.c_str(), port, blocking);
}
