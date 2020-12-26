

#pragma once

#include "IZone.h"
#include "Math/MathFwd.h"
#include <vector>

namespace mono
{
    class ZoneBase : public IZone
    {
    public:

        ~ZoneBase();

        void PostUpdate() override
        { }

        void AddEntity(IEntity* entity, int layer) override;
        void RemoveEntity(IEntity* entity) override;
        
        void AddUpdatable(IUpdatable* updatable) override;
        void RemoveUpdatable(IUpdatable* updatable) override;

        void AddDrawable(IDrawable* drawable, int layer) override;
        void RemoveDrawable(IDrawable* drawable) override;

        void SetDrawableLayer(const IDrawable* drawable, int new_layer) override;

    protected:

        void Accept(IRenderer& renderer) override;
        void Accept(mono::IUpdater& updater) override;
        void DoDeferredDelete();

        std::vector<mono::IEntity*> m_entities;
        std::vector<mono::IUpdatable*> m_updatables;
        std::vector<std::pair<int, mono::IDrawable*>> m_drawables;

        std::vector<mono::IEntity*> m_entities_remove;
        std::vector<mono::IUpdatable*> m_updatables_remove;
        std::vector<mono::IDrawable*> m_drawables_remove;
    };
}
