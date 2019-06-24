
#include "Network.h"
#include <stdexcept>
#include <cstdio>

#define ZED_NET_IMPLEMENTATION
#define ZED_NET_STATIC

#include "zed_net/zed_net.h"
#include <ifaddrs.h>
#include <net/if.h>

#include <errno.h>

namespace
{
    static int s_socket_range_start;
    static int s_socket_range_end;

    class UDPSocket : public network::ISocket
    {
    public:

        UDPSocket(network::SocketType socket_type, unsigned int port)
            : m_port(port)
        {
            const bool non_blocking = (socket_type == network::SocketType::NON_BLOCKING);
            m_handle = zed_net_udp_socket_open(port, non_blocking);
            if(!m_handle)
            {
                char buffer[128];
                std::sprintf(
                    buffer, "network|Failed to create socket on port %u, zed_net_error: %s\n", port, zed_net_get_error());
                throw std::runtime_error(buffer);
            }

            constexpr int yes = 1;
            //const int result1 = setsockopt(m_handle->handle, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
            //if(result1 == -1)
            //    throw std::runtime_error("network|Unable to set reuse address option");

            const int result2 = setsockopt(m_handle->handle, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));
            if(result2 == -1)
                throw std::runtime_error("network|Unable to set broadcast option\n");
        }

        ~UDPSocket()
        {
            zed_net_udp_socket_close(m_handle);
        }

        unsigned short Port() const override
        {
            return m_port;
        }

        bool Send(const std::vector<byte>& data, const network::Address& destination) override
        {
            const int size = static_cast<int>(data.size());

            zed_net_address_t socket_address;
            socket_address.host = destination.host;
            socket_address.port = destination.port;

            const int send_result = zed_net_udp_socket_send(m_handle, socket_address, data.data(), size);
            if(send_result != 0)
            {
                std::printf("network|%s\n", zed_net_get_error());
                std::printf("network|%u\n", errno);
            }

            return send_result == 0;
        }

        bool Receive(std::vector<byte>& data, network::Address* out_sender) override
        {
            zed_net_address_t sender;
            const int size = static_cast<int>(data.capacity());
            const int receive_result = zed_net_udp_socket_receive(m_handle, &sender, data.data(), size);
            if(receive_result < 0)
                std::printf("network|%s\n", zed_net_get_error());

            if(out_sender)
            {
                out_sender->host = sender.host;
                out_sender->port = sender.port;
            }

            return receive_result > 0;
        }

        const unsigned short m_port;
        zed_net_udp_socket_t* m_handle;
    };
}


void network::Initialize(int socket_port_start, int socket_port_end)
{
    const int result = zed_net_init();
    if(result != 0)
        throw std::runtime_error("network|Unable to initialize network");

    const int port_diff = socket_port_end - socket_port_start;
    if(port_diff <= 0)
        throw std::runtime_error("network|Invalid port range");

    s_socket_range_start = socket_port_start;
    s_socket_range_end = socket_port_end;
    
    const std::string& localhost_name = GetLocalhostName();
    const std::string& broadcast_address = AddressToString(GetBroadcastAddress(0));
    const std::string& loopback_address = AddressToString(GetLoopbackAddress(0));

    std::printf("Network\n");

    std::printf("\tport range: %d - %d\n", s_socket_range_start, s_socket_range_end);
    std::printf("\tlocalhost: %s\n", localhost_name.c_str());
    std::printf("\tbroadcast: %s\n", broadcast_address.c_str());
    std::printf("\tloopback: %s\n", loopback_address.c_str());
}

void network::Shutdown()
{
    zed_net_shutdown();
}

std::string network::GetLocalhostName()
{
    constexpr int buffer_size = 128;
    char buffer[buffer_size];
    gethostname(buffer, buffer_size);

    return buffer;
}

network::Address network::MakeAddress(const char* host, unsigned short port)
{
    zed_net_address_t zed_address;
    const int address_result = zed_net_get_address(&zed_address, host, port);
    if(address_result != 0)
        throw std::runtime_error("network|Unable to resolve host address");

    return { zed_address.host, zed_address.port };
}

static network::Address FindSystemAddress(uint32_t address_type, unsigned short port)
{
    std::string address_string;

    ifaddrs* devices = nullptr;
    const int result = getifaddrs(&devices);
    if(result != 0)
        return network::MakeAddress(nullptr, port);

    for(ifaddrs* it = devices; it != nullptr; it = it->ifa_next)
    {
        if(!it->ifa_addr || it->ifa_addr->sa_family != AF_INET)
            continue;

        const bool broadcast = (it->ifa_flags & address_type);
        if(!broadcast)
            continue;

        const sockaddr_in* broadcast_addr = reinterpret_cast<const sockaddr_in*>(it->ifa_broadaddr);
        address_string = inet_ntoa(broadcast_addr->sin_addr);
        break;
    }

    freeifaddrs(devices);

    return network::MakeAddress(address_string.c_str(), port);
}

network::Address network::GetBroadcastAddress(unsigned short port)
{
    return FindSystemAddress(IFF_BROADCAST, port);
}

network::Address network::GetLoopbackAddress(unsigned short port)
{
    return FindSystemAddress(IFF_LOOPBACK, port);
}

std::string network::AddressToString(const Address& address)
{
    std::string out_address = zed_net_host_to_str(address.host);
    if(address.port != 0)
        out_address += ":" + std::to_string(address.port);
    return out_address;
}

network::ISocketPtr network::CreateUDPSocket(SocketType socket_type)
{
    for(int port = s_socket_range_start; port < s_socket_range_end; ++port)
    {
        network::ISocketPtr new_socket = CreateUDPSocket(socket_type, port);
        if(new_socket)
            return new_socket;
    }

    std::printf(
        "network|Unable to create a socket within the given port range [%d - %d]\n",
        s_socket_range_start,
        s_socket_range_end);
    return nullptr;
}

network::ISocketPtr network::CreateUDPSocket(SocketType socket_type, int port)
{
    try
    {
        auto new_socket = std::make_unique<UDPSocket>(socket_type, port);
        std::printf("network|Created socket on port %u\n", port);
        return new_socket;
    }
    catch(const std::runtime_error& error)
    {
        std::printf("%s", error.what());
    }

    return nullptr;
}
