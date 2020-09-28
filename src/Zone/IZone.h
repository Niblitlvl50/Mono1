
#pragma once

#include "MonoFwd.h"
#include "Rendering/RenderFwd.h"

namespace mono
{
    class IZone
    {
    public:

        virtual ~IZone() = default;

        virtual void Accept(mono::IRenderer& renderer) = 0;
        virtual void Accept(mono::IUpdater& updater) = 0;
        virtual void OnLoad(mono::ICamera* camera) = 0;
        virtual int OnUnload() = 0;

        virtual void AddEntity(mono::IEntity* entity, int layer) = 0;
        virtual void RemoveEntity(mono::IEntity* entity) = 0;

        virtual void AddDrawable(mono::IDrawable* drawable, int layer) = 0;
        virtual void RemoveDrawable(mono::IDrawable* drawable) = 0;

        virtual void AddUpdatable(mono::IUpdatable* updatable) = 0;
        virtual void RemoveUpdatable(mono::IUpdatable* updatable) = 0;

        virtual void SetDrawableLayer(const IDrawable* drawable, int new_layer) = 0;
    };
}
