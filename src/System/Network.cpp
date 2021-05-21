
#include "Network.h"
#include "System.h"
#include <stdexcept>
#include <cstdio>

#define ZED_NET_IMPLEMENTATION
#define ZED_NET_STATIC

#include "zed_net/zed_net.h"
#include <errno.h>

namespace
{
    static uint16_t s_socket_range_start;
    static uint16_t s_socket_range_end;

    class UDPSocket : public network::ISocket
    {
    public:

        UDPSocket(network::SocketType socket_type, uint16_t port)
            : m_port(port)
        {
            const bool non_blocking = (socket_type == network::SocketType::NON_BLOCKING);
            const int open_result = zed_net_udp_socket_open(&m_handle, port, non_blocking);
            if(open_result != 0)
            {
                char buffer[128];
                std::sprintf(
                    buffer, "network|Failed to create socket on port %u, zed_net_error: %s\n", port, zed_net_get_error());
                throw std::runtime_error(buffer);
            }

#ifdef _WIN32
            const DWORD tv = 1000;
#else
            timeval tv;
            tv.tv_sec = 1;
            tv.tv_usec = 0;
#endif
            const int set_timeout_result = setsockopt(m_handle.handle, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(timeval));
            if(set_timeout_result == -1)
            {
                char buffer[128];
                std::sprintf(buffer, "network|Unable to set socket timeout. Error: %d\n", set_timeout_result);
                throw std::runtime_error(buffer);
            }

            constexpr int yes = 1;
            //const int result1 = setsockopt(m_handle->handle, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
            //if(result1 == -1)
            //    throw std::runtime_error("network|Unable to set reuse address option");

            const int result2 = setsockopt(m_handle.handle, SOL_SOCKET, SO_BROADCAST, (const char*)&yes, sizeof(yes));
            if(result2 == -1)
                throw std::runtime_error("network|Unable to set broadcast option");
        }

        ~UDPSocket()
        {
            zed_net_socket_close(&m_handle);
        }

        uint16_t Port() const override
        {
            return m_port;
        }

        bool Send(const std::vector<byte>& data, const network::Address& destination) override
        {
            return Send(data.data(), data.size(), destination);
        }

        bool Send(const byte* data, uint32_t size, const network::Address& destination) override
        {
            zed_net_address_t socket_address;
            socket_address.host = destination.host;
            socket_address.port = destination.port;

            const int send_result = zed_net_udp_socket_send(&m_handle, socket_address, data, size);
            if(send_result != 0)
                System::Log("network|%s(%u)", zed_net_get_error(), errno);

            return send_result == 0;
        }

        int Receive(std::vector<byte>& data, network::Address* out_sender) override
        {
            zed_net_address_t sender;
            const int size = static_cast<int>(data.size());
            const int receive_result = zed_net_udp_socket_receive(&m_handle, &sender, data.data(), size);
            if(receive_result < 0)
                System::Log("network|%s", zed_net_get_error());

            if(out_sender)
            {
                out_sender->host = sender.host;
                out_sender->port = sender.port;
            }

            return receive_result;
        }

        const uint16_t m_port;
        zed_net_socket_t m_handle;
    };
}


void network::Initialize(uint16_t socket_port_start, uint16_t socket_port_end)
{
    const int result = zed_net_init();
    if(result != 0)
        throw std::runtime_error("network|Unable to initialize network");

    const uint16_t port_diff = socket_port_end - socket_port_start;
    if(port_diff <= 0)
        throw std::runtime_error("network|Invalid port range");

    s_socket_range_start = socket_port_start;
    s_socket_range_end = socket_port_end;
    
    const std::string& localhost_name = GetLocalhostName();
    const std::string& broadcast_address = AddressToString(GetBroadcastAddress(0));

    System::Log("Network");
    System::Log("\tport range: %d - %d", s_socket_range_start, s_socket_range_end);
    System::Log("\tlocalhost: %s", localhost_name.c_str());
    System::Log("\tbroadcast: %s", broadcast_address.c_str());
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

network::Address network::MakeAddress(const char* host, uint16_t port)
{
    zed_net_address_t zed_address;
    const int address_result = zed_net_get_address(&zed_address, host, port);
    if(address_result != 0)
        throw std::runtime_error("network|Unable to resolve host address");

    return { zed_address.host, zed_address.port };
}

/*
static network::Address FindSystemAddress(uint32_t address_type, uint16_t port)
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
*/


network::Address network::GetBroadcastAddress(uint16_t port)
{
    zed_net_interfaceinfo_t interfaces[16];
    const int n_interfaces = zed_net_enumerate_interfaces(interfaces, std::size(interfaces), true, false);
    for(int index = 0; index < n_interfaces; ++index)
    {
        const zed_net_interfaceinfo_t& interfaceinfo = interfaces[index];
        std::printf("%s, %s, %s, %u\n", interfaceinfo.address, interfaceinfo.interface_name, interfaceinfo.netmask, interfaceinfo.is_ipv6);
    }

    return network::MakeAddress(interfaces[0].address, port);

    //return FindSystemAddress(IFF_BROADCAST, port);
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

    System::Log(
        "network|Unable to create a socket within the given port range [%d - %d]",
        s_socket_range_start,
        s_socket_range_end);
    return nullptr;
}

network::ISocketPtr network::CreateUDPSocket(SocketType socket_type, uint16_t port)
{
    try
    {
        auto new_socket = std::make_unique<UDPSocket>(socket_type, port);
        System::Log("network|Created socket on port %u", port);
        return new_socket;
    }
    catch(const std::runtime_error& error)
    {
        System::Log("%s", error.what());
    }

    return nullptr;
}
