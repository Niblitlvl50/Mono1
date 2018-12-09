
#pragma once

#include "MonoPtrFwd.h"
#include "Rendering/RenderPtrFwd.h"

namespace mono
{
    class IZone
    {
    public:

        virtual ~IZone()
        { }

        virtual void Accept(mono::IRenderer& renderer) = 0;
        virtual void Accept(mono::IUpdater& updater) = 0;
        virtual void OnLoad(mono::ICameraPtr& camera) = 0;
        virtual int OnUnload() = 0;

        virtual void AddEntity(const mono::IEntityPtr& entity, int layer) = 0;
        virtual void RemoveEntity(const mono::IEntityPtr& entity) = 0;

        virtual void AddDrawable(const mono::IDrawablePtr& drawable, int layer) = 0;
        virtual void RemoveDrawable(const mono::IDrawablePtr& drawable) = 0;

        virtual void AddUpdatable(const mono::IUpdatablePtr& updatable) = 0;
        virtual void RemoveUpdatable(const mono::IUpdatablePtr& updatable) = 0;

        virtual void SetDrawableLayer(const IDrawablePtr& drawable, int new_layer) = 0;
        virtual mono::IEntityPtr FindEntityFromId(unsigned int id) const = 0;
    };
}
