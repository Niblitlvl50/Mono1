//
//  AudioFactory.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/01/16.
//
//

#include "AudioFactory.h"
#include "SysTypes.h"
#include "SysFile.h"

#include "OpenAL/al.h"
#include "OpenAL/alc.h"

#include <map>
#include <vector>

#include <iostream>

namespace
{
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

        SoundImpl(const std::shared_ptr<SoundData>& data)
            : m_data(data),
              m_source(0)
        {
            alGenSources(1, &m_source);
            alSourcei(m_source, AL_BUFFER, m_data->BufferId());
            alSourcef(m_source, AL_PITCH, 1.0f);
            alSourcef(m_source, AL_GAIN, 1.0f);
            alSourcei(m_source, AL_LOOPING, AL_FALSE);
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

        std::shared_ptr<SoundData> m_data;
        ALuint m_source;
    };

    std::map<std::string, std::weak_ptr<SoundData>> repository;

    class SoundDataDeleter
    {
    public:

        SoundDataDeleter(const std::string& source)
            : m_source(source)
        { }

        void operator ()(SoundData* ptr)
        {
            repository.erase(m_source);
            delete ptr;
        }

        const std::string m_source;
    };
}

mono::ISoundPtr mono::AudioFactory::CreateSound(const std::string& file)
{
    auto it = repository.find(file);
    if(it != repository.end())
    {
        auto data = it->second.lock();
        if(data)
            return std::make_shared<SoundImpl>(data);

        std::cout << "Unable to create a shared from a weak pointer, will reload the data. Source: " << file << std::endl;
    }

    const mono::SoundFile& soundFile = LoadFile(file);

    auto data = std::shared_ptr<SoundData>(new SoundData(soundFile.data, soundFile.format, soundFile.frequency), SoundDataDeleter(file));
    repository[file] = data;

    return std::make_shared<SoundImpl>(data);
}

/*
 * Struct that holds the RIFF data of the Wave file.
 * The RIFF data is the meta data information that holds,
 * the ID, size and format of the wave file
 */
struct RIFF_Header
{
    char chunkID[4];
    std::int32_t chunkSize;//size not including chunkSize or chunkID
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

mono::SoundFile mono::AudioFactory::LoadFile(const std::string& fileName)
{
    File::FilePtr soundFile = File::OpenBinaryFile(fileName);

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

    //constexpr byte riff[] = {'R', 'I', 'F', 'F'};
    //constexpr byte wave[] = {'W', 'A', 'V', 'E'};
    //constexpr byte fmt_[] = {'f', 'm', 't', ' '};
    //constexpr byte data[] = {'d', 'a', 't', 'a'};

    SoundFile output;
    output.frequency = waveFormat.sampleRate;

    if(waveFormat.numChannels == 1)
        output.format = waveFormat.bitsPerSample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    else
        output.format = waveFormat.bitsPerSample == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;

    bytes.erase(bytes.begin(), bytes.begin() + dataOffset2);
    output.data = std::move(bytes);

    return output;

    /*
    const bool foundRiff = std::search(bytes.begin(), bytes.end(), riff, riff +3) != bytes.end();
    const bool foundWave = std::search(bytes.begin(), bytes.end(), wave, wave +3) != bytes.end();
    const bool foundFmt_ = std::search(bytes.begin(), bytes.end(), fmt_, fmt_ +3) != bytes.end();
    const bool foundData = std::search(bytes.begin(), bytes.end(), data, data +3) != bytes.end();

    if(foundRiff)
        std::cout << "Found RIFF header" << std::endl;

    if(foundWave)
        std::cout << "Found WAVE header" << std::endl;

    if(foundFmt_)
        std::cout << "Found fmt_ header" << std::endl;

    if(foundData)
        std::cout << "Found data header" << std::endl;
     */

    /*
     
     //Local Declarations
     WAVE_Format wave_format;
     RIFF_Header riff_header;
     WAVE_Data wave_data;
     unsigned char* data;

    // Read in the first chunk into the struct
    fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

    //check for RIFF and WAVE tag in memeory
    if ((riff_header.chunkID[0] != 'R' ||
         riff_header.chunkID[1] != 'I' ||
         riff_header.chunkID[2] != 'F' ||
         riff_header.chunkID[3] != 'F') ||
        (riff_header.format[0] != 'W' ||
         riff_header.format[1] != 'A' ||
         riff_header.format[2] != 'V' ||
         riff_header.format[3] != 'E'))
        throw ("Invalid RIFF or WAVE Header");

    //Read in the 2nd chunk for the wave info
    fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
    //check for fmt tag in memory
    if (wave_format.subChunkID[0] != 'f' ||
        wave_format.subChunkID[1] != 'm' ||
        wave_format.subChunkID[2] != 't' ||
        wave_format.subChunkID[3] != ' ')
        throw ("Invalid Wave Format");

    //check for extra parameters;
    if (wave_format.subChunkSize > 16)
        fseek(soundFile, sizeof(short), SEEK_CUR);

    //Read in the the last byte of data before the sound file
    fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
    //check for data tag in memory
    if (wave_data.subChunkID[0] != 'd' ||
        wave_data.subChunkID[1] != 'a' ||
        wave_data.subChunkID[2] != 't' ||
        wave_data.subChunkID[3] != 'a')
        throw ("Invalid data header");

    //Allocate memory for data
    data = new unsigned char[wave_data.subChunk2Size];

    // Read in the sound data into the soundData variable
    if (!fread(data, wave_data.subChunk2Size, 1, soundFile))
        throw ("error loading WAVE data into struct!");

    //Now we set the variables that we passed in with the
    //data from the structs
    wave_data.subChunk2Size;
    wave_format.sampleRate;


    int format;

    //The format is worked out by looking at the number of
    //channels and the bits per sample.
    if (wave_format.numChannels == 1)
    {
        if (wave_format.bitsPerSample == 8 )
            format = AL_FORMAT_MONO8;
        else if (wave_format.bitsPerSample == 16)
            format = AL_FORMAT_MONO16;
    }
    else if(wave_format.numChannels == 2)
    {
        if (wave_format.bitsPerSample == 8 )
            format = AL_FORMAT_STEREO8;
        else if (wave_format.bitsPerSample == 16)
            format = AL_FORMAT_STEREO16;
    }
     */

    //create our openAL buffer and check for success
    //alGenBuffers(1, buffer);

    //now we put our data into the openAL buffer and
    //check for success
    //alBufferData(*buffer, *format, (void*)data, *size, *frequency);
}

