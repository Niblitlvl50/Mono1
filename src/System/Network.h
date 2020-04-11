
#pragma once

#include <vector>
#include <memory>
#include <string>
#include <cstdint>

using byte = unsigned char;

namespace network
{
    struct Address
    {
        uint32_t host;
        uint16_t port;
    };

    enum class SocketType
    {
        BLOCKING,
        NON_BLOCKING
    };

    // Initialize the network module
    void Initialize(uint16_t socket_port_start, uint16_t socket_port_end);

    // Exit the network module
    void Shutdown();

    Address MakeAddress(const char* host, uint16_t port);
    Address GetBroadcastAddress(uint16_t port);
    Address GetLoopbackAddress(uint16_t port);

    std::string GetLocalhostName();
    std::string AddressToString(const Address& address);

    class ISocket
    {
    public:

        virtual ~ISocket() = default;

        // Get the port that this socket is bound to.
        virtual uint16_t Port() const = 0;

        // Send to a specific address and port
        virtual bool Send(const std::vector<byte>& data, const Address& destination) = 0;
        virtual bool Send(const byte* data, uint32_t size, const Address& destination) = 0;

        // Receives data from the socket into the buffer, make sure to reserve
        // the amount of data you want to be able to receive and returns the number
        // of bytes received, less than zero on error.
        virtual int Receive(std::vector<byte>& buffer, Address* out_sender = nullptr) = 0;
    };

    using ISocketPtr = std::unique_ptr<ISocket>;

    // Will return nullptr on failure.
    ISocketPtr CreateUDPSocket(SocketType socket_type);
    ISocketPtr CreateUDPSocket(SocketType socket_type, uint16_t port);

    inline bool operator == (const network::Address& left, const network::Address& right)
    {
        return left.host == right.host && left.port == right.port;
    }
}

namespace std
{
    template <>
    struct hash<network::Address>
    {
        size_t operator()(const network::Address& key) const
        {
            return key.host ^ key.port;
        }
    };
}
