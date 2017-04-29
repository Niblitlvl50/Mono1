
#pragma once

#include <vector>
#include <string>

namespace mono
{
    struct Frame
    {
        int frame = 0;
        int duration = 0;
    };

    class AnimationSequence
    {
    public:

        AnimationSequence(const char* name, bool loop_sequence);

        void AddFrame(int frame_number, int duration);
        void RemoveFrame(int frame_number);

        void Update(unsigned int delta);

        int Frame() const;
        bool Finished() const;
        void Restart();

        bool IsLooping() const;
        void SetLooping(bool state);

        const char* GetName() const;
        void SetName(const char* name);

        const std::vector<mono::Frame>& GetFrames() const;
              std::vector<mono::Frame>& GetFrames();

    private:

        bool m_loop;
        std::string m_name;
        std::vector<mono::Frame> m_frames;

        unsigned int m_currentFrame;
        int m_elapsedTime;
        bool m_done;
    };
}
