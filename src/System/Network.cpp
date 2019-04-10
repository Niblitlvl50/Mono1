
#include "Network.h"
#include <stdexcept>
#include <cstdio>

#define ZED_NET_IMPLEMENTATION
#define ZED_NET_STATIC

#include "zed_net/zed_net.h"
#include <ifaddrs.h>
#include <net/if.h>

namespace
{
    class UDPSocket : public network::ISocket
    {
    public:

        UDPSocket(const char* address, int port, bool blocking)
        {
            const int address_result = zed_net_get_address(&m_address, address, port);
            if(address_result != 0)
                throw std::runtime_error("network|Unable to resolve host address");

            m_handle = zed_net_udp_socket_open(port, !blocking);
            if(!m_handle)
                throw std::runtime_error("network|Failed to create socket");

            //constexpr int yes = 1;
            //const int result1 = setsockopt(m_handle->handle, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
            //if(result1 == -1)
            //    throw std::runtime_error("network|Unable to set reuse address option");

            //const int result2 = setsockopt(m_handle->handle, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));
            //if(result2 == -1)
            //    throw std::runtime_error("network|Unable to set broadcast option");
        }

        ~UDPSocket()
        {
            zed_net_udp_socket_close(m_handle);
        }

        bool Send(const std::vector<byte>& data) override
        {
            const int size = static_cast<int>(data.size());
            const int send_result = zed_net_udp_socket_send(m_handle, m_address, data.data(), size);
            if(send_result != 0)
                std::printf("network|%s\n", zed_net_get_error());

            return send_result == 0;
        }

        bool SendTo(const std::vector<byte>& data, const char* address, int port) override
        {
            const int size = static_cast<int>(data.size());

            zed_net_address_t socket_address;
            const int address_result = zed_net_get_address(&socket_address, address, port);
            if(address_result != 0)
            {
                std::printf("network|%s\n", zed_net_get_error());
                return -1;
            }

            const int send_result = zed_net_udp_socket_send(m_handle, socket_address, data.data(), size);
            if(send_result != 0)
                std::printf("network|%s\n", zed_net_get_error());

            return send_result == 0;
        }

        bool SendTo(const std::vector<byte>& data, const network::Address& destination) override
        {
            return SendTo(data, destination.host.c_str(), destination.port);
        }

        bool Receive(std::vector<byte>& data, network::Address* out_sender) override
        {
            zed_net_address_t sender;
            const int size = static_cast<int>(data.capacity());
            const int receive_result = zed_net_udp_socket_receive(m_handle, &sender, data.data(), size);

            if(out_sender)
            {
                out_sender->host = zed_net_host_to_str(sender.host);
                out_sender->port = sender.port;
            }

            if(receive_result < 0)
                std::printf("network|%s\n", zed_net_get_error());

            return receive_result > 0;
        }

        zed_net_address_t m_address;
        zed_net_udp_socket_t* m_handle;
    };
}


void network::Initialize()
{
    const int result = zed_net_init();
    if(result != 0)
        throw std::runtime_error("network|Unable to initialize network");

    std::printf("Network\n");
    std::printf("\tInitialized.\n");
}

void network::Shutdown()
{
    zed_net_shutdown();
}

std::string network::GetLocalHostName()
{
    constexpr int buffer_size = 128;
    char buffer[buffer_size];
    gethostname(buffer, buffer_size);

    return buffer;
}

std::string network::GetBroadcastAddress()
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

std::string network::GetLoopbackAddress()
{
    std::string loopback_address;

    ifaddrs* devices = nullptr;
    const int result = getifaddrs(&devices);
    if(result != 0)
        return loopback_address;

    for(ifaddrs* it = devices; it != nullptr; it = it->ifa_next)
    {
        if(!it->ifa_addr || it->ifa_addr->sa_family != AF_INET)
            continue;

        const bool loopback = (it->ifa_flags & IFF_LOOPBACK);
        if(!loopback)
            continue;

        const sockaddr_in* broadcast_addr = reinterpret_cast<const sockaddr_in*>(it->ifa_broadaddr);
        loopback_address = inet_ntoa(broadcast_addr->sin_addr);
        break;
    }

    freeifaddrs(devices);

    return loopback_address;        
}

network::ISocketPtr network::CreateUDPSocket(int port, bool blocking)
{
    return OpenUDPSocket(nullptr, port, blocking);
}

network::ISocketPtr network::OpenUDPSocket(const char* address, int port, bool blocking)
{
    try
    {
       return std::make_unique<UDPSocket>(address, port, blocking);
    }
    catch(const std::runtime_error& error)
    {
        std::printf("network|%s\n", error.what());
        std::printf("network|zed_net_error: %s", zed_net_get_error());
    }

    return nullptr;
}

network::ISocketPtr network::OpenBroadcastSocket(int port, bool blocking)
{
    const std::string& broadcast_string = GetBroadcastAddress();
    return OpenUDPSocket(broadcast_string.c_str(), port, blocking);
}

network::ISocketPtr network::OpenLoopbackSocket(int port, bool blocking)
{
    const std::string& loopback_address = GetLoopbackAddress();
    return OpenUDPSocket(loopback_address.c_str(), port, blocking);    
}
