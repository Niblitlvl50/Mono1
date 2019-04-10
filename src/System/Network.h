
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
    std::string GetBroadcastAddress();
    std::string GetLoopbackAddress();

    struct Address
    {
        std::string host;
        unsigned short port;
    };

    class ISocket
    {
    public:

        virtual ~ISocket()
        { }

        // Send data on the socket
        virtual bool Send(const std::vector<byte>& data) = 0;

        // Send to a specific address and port
        virtual bool SendTo(const std::vector<byte>& data, const char* address, int port) = 0;
        virtual bool SendTo(const std::vector<byte>& data, const Address& destination) = 0;

        // Receives data from the socket into the buffer, make sure to reserve
        // the amount of data you want to be able to receive.
        virtual bool Receive(std::vector<byte>& buffer, Address* out_sender = nullptr) = 0;
    };

    using ISocketPtr = std::unique_ptr<ISocket>;

    ISocketPtr CreateUDPSocket(int port, bool blocking);
    ISocketPtr OpenUDPSocket(const char* address, int port, bool blocking);
    ISocketPtr OpenBroadcastSocket(int port, bool blocking);
    ISocketPtr OpenLoopbackSocket(int port, bool blocking);
}
