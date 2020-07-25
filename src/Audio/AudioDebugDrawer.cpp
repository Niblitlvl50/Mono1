
#include "AudioDebugDrawer.h"
#include "Math/Quad.h"

#include "Rendering/Color.h"
#include "Rendering/IRenderer.h"

#include "open_al.h"

using namespace mono;

AudioDebugDrawer::AudioDebugDrawer(const bool& enabled)
    : m_enabled(enabled)
{ }

void AudioDebugDrawer::Draw(mono::IRenderer& renderer) const
{
    if(!m_enabled)
        return;

    math::Vector position;
    math::Vector velocity;
    math::Vector at;

    float al_orientation[6] = { };

    float dummy;
    alGetListener3f(AL_POSITION, &position.x, &position.y, &dummy);
    alGetListener3f(AL_VELOCITY, &velocity.x, &velocity.y, &dummy);
    alGetListenerfv(AL_ORIENTATION, al_orientation);

    at.x = al_orientation[0];
    at.y = al_orientation[1];

    renderer.DrawLines({ position, position + velocity }, mono::Color::MAGENTA, 2.0f);
    renderer.DrawLines({ position, position + at }, mono::Color::RED, 2.0f);
    renderer.DrawPoints({ position }, mono::Color::CYAN, 4.0f);
}

math::Quad AudioDebugDrawer::BoundingBox() const
{
    return math::InfQuad;
}
