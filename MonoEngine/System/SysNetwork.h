//
//  SysNetwork.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 22/01/16.
//
//

#pragma once

#include "SysTypes.h"
#include <vector>
#include <memory>
#include <string>

namespace Network
{
    //! Initialize the network module
    void Init();

    //! Exit the network module
    void Exit();

    std::string GetLocalHostName();


    class ISocket
    {
    public:

        virtual ~ISocket()
        { }

        //! Send data on the socket
        virtual int Send(const std::vector<byte>& data) = 0;

        //! Send to a specific address and port
        virtual int SendTo(const std::vector<byte>& data, const char* address, int port) = 0;

        //! Receives data from the socket into the buffer,
        //! make sure to reserve the amount of data you want to
        //! be able to receive.
        virtual void Receive(std::vector<byte>& buffer) = 0;
    };

    std::shared_ptr<ISocket> CreateUDPSocket(int port, bool blocking);
    std::shared_ptr<ISocket> OpenUDPSocket(const char* address, int port, bool blocking);
    std::shared_ptr<ISocket> OpenBroadcastSocket(int port, bool blocking);
}
