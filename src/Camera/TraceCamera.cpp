
#include "TraceCamera.h"
#include "Zone/IEntity.h"
#include "Math/Vector.h"
#include "Math/MathFunctions.h"

#include <cmath>

using namespace mono;

namespace constants
{
    constexpr float SPEED = 0.01f;
}

TraceCamera::TraceCamera(int width, int height)
    : m_offset(math::ZeroVec),
      mViewport(0.0f, 0.0f, width, height),
      mTargetViewport(mViewport)
{ }

void TraceCamera::doUpdate(const UpdateContext& update_context)
{
    const float change = (mTargetViewport.mB.x - mViewport.mB.x);
    const float xzero = std::abs(change);

    if(xzero != 0.0f)
    {
        const float aspect = mViewport.mB.x / mViewport.mB.y;
        math::ResizeQuad(mViewport, change * 0.1f, aspect);
    }
    
    if(mEntity)
    {
        const math::Vector& targetPosition = mEntity->Position() - (mViewport.mB * 0.5f);
        const math::Vector& diff = targetPosition - mViewport.mA;
    
        const math::Vector& move = diff * (update_context.delta_ms * constants::SPEED);
        mViewport.mA += move;
    }
}

void TraceCamera::Follow(uint32_t entity_id, const math::Vector& offset)
{

}

void TraceCamera::Unfollow()
{
    mEntity = nullptr;
}

math::Quad TraceCamera::GetViewport() const
{
    math::Quad result = mViewport;
    result.mA -= m_offset;

    return result;
}

void TraceCamera::SetViewport(const math::Quad& viewport)
{
    mViewport = viewport;
    mTargetViewport = viewport;
}

void TraceCamera::SetTargetViewport(const math::Quad& target)
{
    mTargetViewport = target;
}

void TraceCamera::SetPosition(const math::Vector& position)
{
    // The position is the middle of the quad, not the lower left corner.
    const math::Vector& xy = position - (mViewport.mB * 0.5f);
    mViewport.mA = xy;
}

math::Vector TraceCamera::GetPosition() const
{
    return mViewport.mA + (mViewport.mB * 0.5f);
}

math::Vector TraceCamera::ScreenToWorld(const math::Vector& screen_pos, const math::Vector& window_size) const
{
    const float ratio = window_size.x / window_size.y;

    math::Quad viewport = GetViewport();
    viewport.mB.y = viewport.mB.x / ratio;

    const math::Vector& scale = viewport.mB / window_size;
    
    const float screenX = screen_pos.x;
    const float screenY = window_size.y - screen_pos.y;
    
    const float tempx = screenX * scale.x;
    const float tempy = screenY * scale.y;
    
    return math::Vector(tempx + viewport.mA.x, tempy + viewport.mA.y);
}
