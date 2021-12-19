

#pragma once

#include "IZone.h"
#include "Math/MathFwd.h"
#include <vector>

namespace mono
{
    struct DrawableData
    {
        mono::IDrawable* drawable;
        int layer;
    };

    class ZoneBase : public IZone
    {
    public:

        void PostUpdate() override
        { }

        void AddUpdatable(IUpdatable* updatable) override;
        void RemoveUpdatable(IUpdatable* updatable) override;

        void AddDrawable(IDrawable* drawable, int layer) override;
        void RemoveDrawable(IDrawable* drawable) override;

        void SetDrawableLayer(const IDrawable* drawable, int new_layer) override;
        void SetLastLightingLayer(int layer) override;

    protected:

        int OnUnload() override;
        void Accept(IRenderer& renderer) override;
        void Accept(mono::IUpdater& updater) override;
        void DoDeferredDelete();

        std::vector<mono::IUpdatable*> m_updatables;
        std::vector<DrawableData> m_drawables;

        std::vector<mono::IUpdatable*> m_updatables_remove;
        std::vector<mono::IDrawable*> m_drawables_remove;

        int m_last_lighting_layer = 0;
    };
}
