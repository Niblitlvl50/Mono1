
#include <gtest/gtest.h>
#include "Audio/AudioFactory.h"

TEST(Sound, LoadWaveAndExpectSuccess)
{
    const mono::SoundFile& file = mono::AudioFactory::LoadFile("UnitTests/TestFiles/a.wav");

    EXPECT_EQ(file.format, mono::SoundFormat::MONO_8BIT);
    EXPECT_EQ(file.frequency, 11025);
    EXPECT_EQ(file.data.size(), 1793ul);
}

TEST(Sound, LoadMissingFileExpectException)
{
    EXPECT_THROW(mono::AudioFactory::LoadFile("whatever.asdf"), std::runtime_error);
}
