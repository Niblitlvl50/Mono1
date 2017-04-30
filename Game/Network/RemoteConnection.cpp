
#include "RemoteConnection.h"
#include "System/Network.h"
#include "System/System.h"

using namespace game;

RemoteConnection::RemoteConnection()
    : m_stop(false),
      m_broadcast(true)
{
    const bool& stop = m_stop;
    const bool& broadcast = m_broadcast;

    const auto comm_func = [&stop]() {
        Network::ISocketPtr comm_socket = Network::CreateUDPSocket(16666, false); 
        std::vector<byte> data(1024, '\0');

        while(!stop)
        {
            const bool got_data = comm_socket->Receive(data);
            if(got_data)
            {
                std::printf("incomming data: %s\n", data.data());
                std::fill(data.begin(), data.begin() + data.size(), '\0');
            }
            else
            {
                System::Sleep(10);
            }
        }
    };

    const auto broadcast_func = [&stop, &broadcast]() {
        Network::ISocketPtr broadcast_socket = Network::OpenBroadcastSocket(17777, false);
        const std::vector<byte> data = { 'm', 'o', 'n', 'o', '1' };

        while(!stop)
        {
            if(broadcast_socket)
                broadcast_socket->Send(data);
            System::Sleep(1000);
        }
    };

    m_commThread = std::thread(comm_func);
    m_broadcastThread = std::thread(broadcast_func);
}

RemoteConnection::~RemoteConnection()
{
    m_stop = true;
    m_commThread.join();
    m_broadcastThread.join();
}

void RemoteConnection::SetBroadcast(bool enable)
{
    m_broadcast = enable;
}
