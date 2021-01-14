
#include <gtest/gtest.h>
#include "System/Network.h"

TEST(Network, InitializeShutdow)
{
    network::Initialize(0, 10);
    network::Shutdown();
}
