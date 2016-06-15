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

namespace Network
{
    //! Initialize the network module
    void Init();

    //! Exit the network module
    void Exit();


    class ISocket
    {
    public:

        virtual ~ISocket()
        { }

        //! Send data on the socket
        virtual int Send(const std::vector<byte>& data) = 0;

        //! Receives data from the socket into the buffer,
        //! make sure to reserve the amount of data you want to
        //! be able to receive.
        virtual void Receive(std::vector<byte>& buffer) = 0;
    };

    //! Create a UDP socket...
    std::shared_ptr<ISocket> OpenUDPSocket(const char* address, int port, bool blocking);
}
