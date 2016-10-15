//
//  AudioFactory.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/01/16.
//
//

#include "AudioFactory.h"
#include "System/SysTypes.h"
#include "System/SysFile.h"

#include "OpenAL/al.h"

#include <unordered_map>
#include <string>
#include <exception>
#include <cstdio>


namespace
{
    /*
     * Struct that holds the RIFF data of the Wave file.
     * The RIFF data is the meta data information that holds,
     * the ID, size and format of the wave file
     */
    struct RIFF_Header
    {
        char chunkID[4];
        std::int32_t chunkSize; //size not including chunkSize or chunkID
        char format[4];
    };

    /*
     * Struct to hold fmt subchunk data for WAVE files.
     */
    struct WAVE_Format
    {
        char subChunkID[4];
        std::int32_t subChunkSize;
        std::int16_t audioFormat;
        std::int16_t numChannels;
        std::int32_t sampleRate;
        std::int32_t byteRate;
        std::int16_t blockAlign;
        std::int16_t bitsPerSample;
    };

    /*
     * Struct to hold the data of the wave file
     */
    struct WAVE_Data
    {
        char subChunkID[4]; //should contain the word data
        std::int32_t subChunk2Size; //Stores the size of the data block
    };

    class SoundData
    {
    public:

        SoundData(const std::vector<byte>& data, ALenum format, ALsizei frequency)
        {
            alGenBuffers(1, &m_buffer);
            
            const ALsizei n = static_cast<ALsizei>(data.size());
            alBufferData(m_buffer, format, data.data(), n, frequency);
        }
        ~SoundData()
        {
            alDeleteBuffers(1, &m_buffer);
        }
        ALuint BufferId() const
        {
            return m_buffer;
        }

    private:

        ALuint m_buffer;
    };

    class SoundImpl : public mono::ISound
    {
    public:

        SoundImpl(const std::shared_ptr<SoundData>& data, bool loop, bool relative)
            : m_data(data),
              m_source(0)
        {
            alGenSources(1, &m_source);
            alSourcei(m_source, AL_BUFFER, static_cast<ALint>(m_data->BufferId()));
            alSourcei(m_source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
            alSourcei(m_source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);

            Pitch(1.0f);
            Gain(1.0f);
        }
        ~SoundImpl()
        {
            Stop();
            alDeleteSources(1, &m_source);
        }
        void Play()
        {
            alSourcePlay(m_source);
        }
        void Pause()
        {
            alSourcePause(m_source);
        }
        void Stop()
        {
            alSourceStop(m_source);
        }
        void Pitch(float pitch)
        {
            alSourcef(m_source, AL_PITCH, pitch);
        }
        void Gain(float gain)
        {
            alSourcef(m_source, AL_GAIN, gain);
        }
        void Position(float x, float y)
        {
            alSource3f(m_source, AL_POSITION, x, y, 0.0f);
        }

        std::shared_ptr<SoundData> m_data;
        ALuint m_source;
    };

    ALenum SoundFormatToALFormat(mono::SoundFormat format)
    {
        if(format == mono::SoundFormat::MONO_8BIT)
            return AL_FORMAT_MONO8;
        else if(format == mono::SoundFormat::MONO_16BIT)
            return AL_FORMAT_MONO16;
        else if(format == mono::SoundFormat::STEREO_8BIT)
            return AL_FORMAT_STEREO8;
        else
            return AL_FORMAT_STEREO16;
    }

    // The sound data repository, this is where all the sounds are stored!
    std::unordered_map<std::string, std::weak_ptr<SoundData>> repository;
}

mono::ISoundPtr mono::AudioFactory::CreateSound(const char* fileName, bool loop, bool relative)
{
    auto it = repository.find(fileName);
    if(it != repository.end())
    {
        auto data = it->second.lock();
        if(data)
            return std::make_shared<SoundImpl>(data, loop, relative);

        std::printf("Unable to create a shared from a weak pointer, will reload the data. Source: %s\n", fileName);
    }

    const mono::SoundFile& soundFile = LoadFile(fileName);
    const ALenum format = SoundFormatToALFormat(soundFile.format);

    // Custom deleter to actualy erase the file from the repository
    // when the last reference goes out of scope.
    const auto deleter = [fileName](SoundData* ptr) {
        repository.erase(fileName);
        delete ptr;
    };

    auto data = std::shared_ptr<SoundData>(new SoundData(soundFile.data, format, soundFile.frequency), deleter);

    // Store it in the repository for others to retreive
    repository[fileName] = data;

    return std::make_shared<SoundImpl>(data, loop, relative);
}

mono::SoundFile mono::AudioFactory::LoadFile(const char* fileName)
{
    File::FilePtr soundFile = File::OpenBinaryFile(fileName);
    if(!soundFile)
        throw std::runtime_error("Unable to open sound file");

    std::vector<byte> bytes;
    File::FileRead(soundFile, bytes);

    RIFF_Header riffHeader;
    WAVE_Format waveFormat;
    WAVE_Data waveData;

    constexpr int formatOffset = sizeof(RIFF_Header);
    constexpr int dataOffset1 = formatOffset + sizeof(WAVE_Format);
    constexpr int dataOffset2 = dataOffset1 + sizeof(WAVE_Data);

    std::memcpy(&riffHeader, bytes.data(), sizeof(RIFF_Header));
    std::memcpy(&waveFormat, bytes.data() + formatOffset, sizeof(WAVE_Format));
    std::memcpy(&waveData,   bytes.data() + dataOffset1, sizeof(WAVE_Data));

    // Use these for error checking...

    //constexpr byte riff[] = {'R', 'I', 'F', 'F'};
    //constexpr byte wave[] = {'W', 'A', 'V', 'E'};
    //constexpr byte fmt_[] = {'f', 'm', 't', ' '};
    //constexpr byte data[] = {'d', 'a', 't', 'a'};

    SoundFile output;
    output.frequency = waveFormat.sampleRate;

    if(waveFormat.numChannels == 1)
        output.format = waveFormat.bitsPerSample == 8 ? SoundFormat::MONO_8BIT : SoundFormat::MONO_16BIT;
    else
        output.format = waveFormat.bitsPerSample == 8 ? SoundFormat::STEREO_8BIT : SoundFormat::STEREO_16BIT;

    // Erase the meta data and keep only the sound data
    bytes.erase(bytes.begin(), bytes.begin() + dataOffset2);
    output.data = std::move(bytes);

    return output;
}

