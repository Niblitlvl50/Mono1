
#pragma once

#include "ICamera.h"
#include "MonoFwd.h"
#include "Math/Quad.h"

namespace mono
{
    class TraceCamera : public mono::ICamera
    {
    public:
        
        TraceCamera(int width, int height);
        
        virtual void doUpdate(const UpdateContext& update_context);
        
        virtual void Follow(uint32_t entity_id, const math::Vector& offset);
        virtual void Unfollow();
        
        virtual math::Quad GetViewport() const;
        virtual math::Vector GetPosition() const;

        virtual void SetViewport(const math::Quad& viewport);
        virtual void SetTargetViewport(const math::Quad& target);
        virtual void SetPosition(const math::Vector& position);
        virtual math::Vector ScreenToWorld(const math::Vector& screen_pos, const math::Vector& window_size) const;

    private:

        IEntityPtr mEntity;
        math::Vector m_offset;
        math::Quad mViewport;
        math::Quad mTargetViewport;
    };
}
