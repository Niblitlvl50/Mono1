//
//  SysNetwork.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 22/01/16.
//
//

#include "SysNetwork.h"
#include <stdexcept>

#define ZED_NET_IMPLEMENTATION
#define ZED_NET_STATIC

#include "zed_net.h"


namespace
{
    class UDPSocket : public Network::ISocket
    {
    public:

        UDPSocket(const std::string& address, int port, bool blocking)
        {
            zed_net_get_address(&m_address, address.c_str(), port);
            m_handle = zed_net_udp_socket_open(port, blocking);
            if(!m_handle)
                throw std::runtime_error("Unable to open UDP socket, error: " + std::string(zed_net_get_error()));
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

        void Receive(std::vector<byte>& data)
        {
            zed_net_address_t sender;
            const int size = static_cast<int>(data.capacity());
            zed_net_udp_socket_receive(m_handle, &sender, data.data(), size);
            data.shrink_to_fit();
        }

        zed_net_address_t m_address;
        zed_net_udp_socket_t* m_handle;
    };
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

std::shared_ptr<Network::ISocket> Network::OpenUDPSocket(const std::string& address, int port, bool blocking)
{
    return std::make_shared<UDPSocket>(address, port, blocking);
}

