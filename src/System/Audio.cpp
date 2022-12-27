
#include "Audio.h"
#include "System/Hash.h"
#include "System/System.h"
#include "System/File.h"

#include <cstdint>
#include <unordered_map>
#include <stdexcept>
#include <string_view>


#ifdef _WIN32
    #include <Windows.h>
    #define _CRT_SECURE_NO_WARNINGS 1
#endif

// #ifdef __linux__
//     #define CUTE_SOUND_FORCE_SDL
// #else
// #endif

//#define CUTE_SOUND_FORCE_SDL
#define CUTE_SOUND_IMPLEMENTATION

#include "stb/stb_vorbis.c"
#include "cute_headers/cute_sound.h"


// #define MINIAUDIO_IMPLEMENTATION
// #include "miniaudio/miniaudio.h"

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
    };

    class ISoundEngine
    {
    public:

        virtual ~ISoundEngine() = default;
        virtual audio::ISoundPtr CreateSound(const char* file_name, audio::SoundPlayback playback) = 0;
    };

    class CuteSoundImpl : public ISoundEngine
    {
    public:

        CuteSoundImpl()
        {
            constexpr int frequency = 44100; // a good standard frequency for playing commonly saved OGG + wav files
            constexpr int buffered_samples = 8192; // number of samples internal buffers can hold at once
            constexpr int num_elements_in_playing_pool = 8; // pooled memory array size for playing sounds

            void* hwnd_on_windows = nullptr;

        #ifdef _WIN32
            hwnd_on_windows = GetForegroundWindow();
            if(!hwnd_on_windows)
                hwnd_on_windows = GetDesktopWindow();
        #endif

            m_context = cs_make_context(hwnd_on_windows, frequency, buffered_samples, num_elements_in_playing_pool, nullptr);
            if(!m_context)
            {
                System::Log("Audio|Failed to initialize audio context! [%s]", cs_error_reason);
                throw std::runtime_error("Failed to initialize audio.");
            }

            cs_thread_sleep_delay(m_context, 5);
            cs_spawn_mix_thread(m_context);
        }

        ~CuteSoundImpl()
        {
            StopAllSounds();
            m_sound_repository.clear();
            cs_shutdown_context(m_context);
        }

        audio::ISoundPtr CreateSound(const char* file_name, audio::SoundPlayback playback) override
        {
            const uint32_t sound_hash = hash::Hash(file_name);
            auto it = m_sound_repository.find(sound_hash);
            if(it != m_sound_repository.end())
            {
                auto loaded_sound = it->second;
                if(loaded_sound)
                    return std::make_unique<SoundInstanceImpl>(m_context, loaded_sound, playback);

                System::Log("Audio|Recreating '%s'.", file_name);
            }

            const auto deleter = [](SoundData* ptr) {
                cs_free_sound(&ptr->sound);
                delete ptr;
            };

            std::shared_ptr<SoundData> loaded_sound(new SoundData, deleter);
            loaded_sound->sound.channels[0] = nullptr;
            loaded_sound->sound.channels[1] = nullptr;

            System::Log("Audio|Loading sound '%s'.", file_name);

            const bool is_wave = file::IsExtension(file_name, "wav");
            if(is_wave)
                loaded_sound->sound = cs_load_wav(file_name);
            else if(file::IsExtension(file_name, "ogg"))
                loaded_sound->sound = cs_load_ogg(file_name);

            if(loaded_sound->sound.channels[0] == nullptr)
            {
                System::Log("Audio|Unable to load audio file '%s' ['%s'].", cs_error_reason, file_name);
                return std::make_unique<NullSound>();
            }

            // Store it in the repository for others to retreive
            m_sound_repository[sound_hash] = loaded_sound;
            return std::make_unique<SoundInstanceImpl>(m_context, loaded_sound, playback);
        }

        void StopAllSounds()
        {
            cs_stop_all_sounds(m_context);
        }

        struct SoundData
        {
            cs_loaded_sound_t sound;
        };

        class SoundInstanceImpl : public audio::ISound
        {
        public:
            
            SoundInstanceImpl(cs_context_t* context, std::shared_ptr<SoundData> data, audio::SoundPlayback playback)
                : m_context(context)
                , m_sound_data(data)
                , m_playing_sound(nullptr)
            {
                m_sound_def = cs_make_def(&m_sound_data->sound);
                m_sound_def.looped = (playback == audio::SoundPlayback::LOOPING);
            }

            ~SoundInstanceImpl()
            {
                Stop();
            }

            void Play() override
            {
                if(IsPlaying() && m_playing_sound->paused != 0)
                    m_playing_sound->paused = 0;
                else
                    m_playing_sound = cs_play_sound(m_context, m_sound_def);
            }

            void Pause() override
            {
                if(IsPlaying())
                {
                    constexpr int one_for_paused = 1;
                    cs_pause_sound(m_playing_sound, one_for_paused);
                }
            }

            void Stop() override
            {
                if(IsPlaying())
                    cs_stop_sound(m_playing_sound);
            }

            bool IsPlaying() const override
            {
                if(m_playing_sound)
                    return cs_is_active(m_playing_sound) != 0;
                return false;
            }

            void SetVolume(float volume) override
            {
                m_sound_def.volume_left = volume;
                m_sound_def.volume_right = volume;

                if(IsPlaying())
                {
                    m_playing_sound->volume0 = volume;
                    m_playing_sound->volume1 = volume;
                }
            }

            cs_context_t* m_context;
            std::shared_ptr<SoundData> m_sound_data;
            cs_playing_sound_t* m_playing_sound;
            cs_play_sound_def_t m_sound_def;
        };

        std::unordered_map<uint32_t, std::shared_ptr<SoundData>> m_sound_repository;
        cs_context_t* m_context = nullptr;
    };


/*
    class MiniAuidioSoundImpl : public ISoundEngine
    {
        class MiniaudioSound : public audio::ISound
        {
        public:
            MiniaudioSound(const ma_sound& sound)
                : m_ma_sound(sound)
            {}

            ~MiniaudioSound()
            {
                ma_sound_uninit(&m_ma_sound);
            }
        
            void Play() override
            {
                //const ma_result result = ma_sound_start(&m_ma_sound);
            }
            void Pause() override
            {
            }
            void Stop() override
            {
                //const ma_result result = ma_sound_stop(&m_ma_sound);
            }
            bool IsPlaying() const override
            {
                return false;
                //return (ma_sound_is_playing(&m_ma_sound) != MA_FALSE);
            }
            void SetVolume(float volume) override
            {
                //ma_sound_set_volume(&m_ma_sound, volume);
            }

            ma_sound m_ma_sound;
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

        audio::ISoundPtr CreateSound(const char* file_name, audio::SoundPlayback playback) override
        {
            ma_sound sound;
            const ma_result result = ma_sound_init_from_file(&m_ma_engine, file_name, MA_SOUND_FLAG_DECODE, nullptr, nullptr, &sound);
            if(result != MA_SUCCESS)
            {
                return std::make_unique<NullSound>();
            }


            return std::make_unique<MiniaudioSound>(sound);
        }

        ma_engine m_ma_engine; 
    };
    */

    ISoundEngine* g_sound_engine = nullptr;
}

void audio::Initialize()
{
    g_sound_engine = new CuteSoundImpl();
    //g_sound_engine = new MiniAuidioSoundImpl();

}

void audio::Shutdown()
{
    delete g_sound_engine;
    g_sound_engine = nullptr;
}

audio::ISoundPtr audio::CreateSound(const char* file_name, audio::SoundPlayback playback)
{
    return g_sound_engine->CreateSound(file_name, playback);
}

audio::ISoundPtr audio::CreateNullSound()
{
    return std::make_unique<NullSound>();
}

void audio::MixSounds()
{
//    if(g_context)
//        cs_mix(g_context);
}
