//
//  SysSound.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/01/16.
//
//

#pragma once

namespace Sound
{
    // Initialize the sounds system
    void Init();

    // Exit the sound system
    void Exit();

    // Set the listener position
    void ListenerPosition(float x, float y);
}
