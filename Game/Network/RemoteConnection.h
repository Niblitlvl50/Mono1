
#pragma once

#include <thread>

namespace game
{
    class RemoteConnection
    {
    public:

        RemoteConnection();
        ~RemoteConnection();

        void SetBroadcast(bool enable);

    private:

        bool m_stop;
        bool m_broadcast;
        std::thread m_commThread;
        std::thread m_broadcastThread;
    };
}
