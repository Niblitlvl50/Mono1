
#pragma once

#include <vector>
#include <memory>
#include <string>

using byte = unsigned char;

namespace network
{
    // Initialize the network module
    void Initialize();

    // Exit the network module
    void Shutdown();

    std::string GetLocalHostName();

    struct Address
    {
        unsigned int host;
        unsigned short port;
    };

    Address MakeAddress(const char* host, unsigned short port);
    Address GetBroadcastAddress(unsigned short port);
    Address GetLoopbackAddress(unsigned short port);

    class ISocket
    {
    public:

        virtual ~ISocket()
        { }

        // Send data on the socket
        virtual bool Send(const std::vector<byte>& data) = 0;

        // Send to a specific address and port
        virtual bool SendTo(const std::vector<byte>& data, const Address& destination) = 0;

        // Receives data from the socket into the buffer, make sure to reserve
        // the amount of data you want to be able to receive.
        virtual bool Receive(std::vector<byte>& buffer, Address* out_sender = nullptr) = 0;
    };

    using ISocketPtr = std::unique_ptr<ISocket>;

    ISocketPtr CreateUDPSocket(int port, bool blocking);
    ISocketPtr OpenUDPSocket(const network::Address& address, bool blocking);
    ISocketPtr OpenBroadcastSocket(int port, bool blocking);
    ISocketPtr OpenLoopbackSocket(int port, bool blocking);
}
