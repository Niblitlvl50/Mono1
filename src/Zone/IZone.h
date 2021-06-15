
#pragma once

#include "MonoFwd.h"
#include "Rendering/RenderFwd.h"

namespace mono
{
    class IZone
    {
    public:

        virtual ~IZone() = default;

        virtual void OnLoad(mono::ICamera* camera, mono::IRenderer* renderer) = 0;
        virtual int OnUnload() = 0;

        virtual void PostUpdate() = 0;

        virtual void Accept(mono::IRenderer& renderer) = 0;
        virtual void Accept(mono::IUpdater& updater) = 0;

        virtual void AddDrawable(mono::IDrawable* drawable, int layer) = 0;
        virtual void RemoveDrawable(mono::IDrawable* drawable) = 0;

        virtual void AddUpdatable(mono::IUpdatable* updatable) = 0;
        virtual void RemoveUpdatable(mono::IUpdatable* updatable) = 0;

        virtual void SetDrawableLayer(const IDrawable* drawable, int new_layer) = 0;

        virtual void SetLastLightingLayer(int layer) = 0;

        template <typename T>
        void AddUpdatableDrawable(T* object, int layer)
        {
            AddUpdatable(object);
            AddDrawable(object, layer);
        }

        template <typename T>
        void RemoveUpdatableDrawable(T* object)
        {
            RemoveUpdatable(object);
            RemoveDrawable(object);
        }
    };
}
