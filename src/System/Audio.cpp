
#include "Audio.h"
#include "System/Hash.h"
#include "System/System.h"
#include "System/File.h"

#include <cstdint>
#include <unordered_map>
#include <stdexcept>
#include <string_view>

#define STB_VORBIS_HEADER_ONLY
#include "stb/stb_vorbis.c"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

namespace
{
    class NullSound : public audio::ISound
    {
    public:
        void Play() override { }
        void Pause() override { }
        void Stop() override { }
        bool IsPlaying() const override { return false; }
        void SetVolume(float volume) override { }
        void SetPosition(float x, float y) override { }
    };

    class ISoundEngine
    {
    public:

        virtual ~ISoundEngine() = default;
        virtual void SetListenerPosition(float x, float y) = 0;
        virtual audio::ISoundPtr CreateSound(const char* file_name, audio::SoundPlayback playback, audio::SoundSpatiality spatiality) = 0;
    };

    class MiniAuidioSoundImpl : public ISoundEngine
    {
        class MiniaudioSound : public audio::ISound
        {
        public:
            MiniaudioSound(ma_sound* sound)
                : m_ma_sound(sound)
            {}

            ~MiniaudioSound()
            {
                ma_sound_uninit(m_ma_sound);
                delete m_ma_sound;
            }
            
            void Play() override
            {
                if(IsPlaying())
                    ma_sound_seek_to_pcm_frame(m_ma_sound, 0);
                else
                    const ma_result result = ma_sound_start(m_ma_sound);
            }
            void Pause() override
            {
            }
            void Stop() override
            {
                const ma_result result = ma_sound_stop(m_ma_sound);
            }
            bool IsPlaying() const override
            {
                return (ma_sound_is_playing(m_ma_sound) != MA_FALSE);
            }
            void SetVolume(float volume) override
            {
                ma_sound_set_volume(m_ma_sound, volume);
            }
            void SetPosition(float x, float y) override
            {
                ma_sound_set_position(m_ma_sound, x, y, 0.0f);
            }

            ma_sound* m_ma_sound;
        };

    public:

        MiniAuidioSoundImpl()
        {
            const ma_result init_result = ma_engine_init(nullptr, &m_ma_engine);
            if(init_result != MA_SUCCESS)
            {
                System::Log("Audio|Failed to initialize ma audio context!");
                throw std::runtime_error("Failed to initialize audio");
            }
        }

        ~MiniAuidioSoundImpl()
        {
            ma_engine_uninit(&m_ma_engine);
        }

        void SetListenerPosition(float x, float y) override
        {
            ma_engine_listener_set_position(&m_ma_engine, 0, x, y, 0.0f);
        }

        audio::ISoundPtr CreateSound(const char* file_name, audio::SoundPlayback playback, audio::SoundSpatiality spatiality) override
        {
            ma_sound* sound = new ma_sound;

            ma_uint32 flags = MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_PITCH;
            if(spatiality == audio::SoundSpatiality::NONE)
                flags |= MA_SOUND_FLAG_NO_SPATIALIZATION;

                const ma_result result = ma_sound_init_from_file(&m_ma_engine, file_name, flags, nullptr, nullptr, sound);
            if(result != MA_SUCCESS)
            {
                ma_sound_uninit(sound);
                delete sound;
                return std::make_unique<NullSound>();
            }

            const bool looping_sound = (playback == audio::SoundPlayback::LOOPING);
            if(looping_sound)
                ma_sound_set_looping(sound, MA_TRUE);
            return std::make_unique<MiniaudioSound>(sound);
        }

        ma_engine m_ma_engine; 
    };

    ISoundEngine* g_sound_engine = nullptr;
}

void audio::Initialize()
{
    g_sound_engine = new MiniAuidioSoundImpl();
}

void audio::Shutdown()
{
    delete g_sound_engine;
    g_sound_engine = nullptr;
}

void audio::SetListenerPosition(float x, float y)
{
    if(g_sound_engine)
        g_sound_engine->SetListenerPosition(x, y);
}

audio::ISoundPtr audio::CreateSound(const char* file_name, audio::SoundPlayback playback, audio::SoundSpatiality spatiality)
{
    if(g_sound_engine)
        return g_sound_engine->CreateSound(file_name, playback, spatiality);

    return CreateNullSound();
}

audio::ISoundPtr audio::CreateNullSound()
{
    return std::make_unique<NullSound>();
}

void audio::MixSounds()
{
}

#undef STB_VORBIS_HEADER_ONLY
#include "stb/stb_vorbis.c"
