//
//  SoundTest.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/01/16.
//
//

#include <gtest/gtest.h>

#include "AudioFactory.h"

TEST(Sound, LoadWave)
{
    const mono::SoundFile& file = mono::AudioFactory::LoadFile("a.wav");

    EXPECT_EQ(file.format, 4352);
    EXPECT_EQ(file.frequency, 11025);
    EXPECT_EQ(file.data.size(), 1793);
}
