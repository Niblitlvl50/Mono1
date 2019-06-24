
#pragma once

#include <vector>
#include <memory>
#include <string>

using byte = unsigned char;

namespace network
{
    struct Address
    {
        unsigned int host;
        unsigned short port;
    };

    enum class SocketType
    {
        BLOCKING,
        NON_BLOCKING
    };

    // Initialize the network module
    void Initialize(int socket_port_start, int socket_port_end);

    // Exit the network module
    void Shutdown();

    std::string GetLocalhostName();
    Address MakeAddress(const char* host, unsigned short port);
    Address GetBroadcastAddress(unsigned short port);
    Address GetLoopbackAddress(unsigned short port);
    std::string AddressToString(const Address& address);

    class ISocket
    {
    public:

        virtual ~ISocket()
        { }

        // Get the port that this socket is bound to.
        virtual unsigned short Port() const = 0;

        // Send to a specific address and port
        virtual bool Send(const std::vector<byte>& data, const Address& destination) = 0;

        // Receives data from the socket into the buffer, make sure to reserve
        // the amount of data you want to be able to receive.
        virtual bool Receive(std::vector<byte>& buffer, Address* out_sender = nullptr) = 0;
    };

    using ISocketPtr = std::unique_ptr<ISocket>;

    // Will return nullptr on failure.
    ISocketPtr CreateUDPSocket(SocketType socket_type);
    ISocketPtr CreateUDPSocket(SocketType socket_type, int port);
}
