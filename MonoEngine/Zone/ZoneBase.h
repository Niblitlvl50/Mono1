

#pragma once

#include "IZone.h"
#include "Math/MathFwd.h"
#include <vector>

namespace mono
{
    class ZoneBase : public virtual IZone
    {

    public:

        virtual void AddEntity(const IEntityPtr& entity, int layer);
        virtual void RemoveEntity(const IEntityPtr& entity);
        
        virtual void AddUpdatable(const IUpdatablePtr& updatable);
        virtual void RemoveUpdatable(const IUpdatablePtr& updatable);
        
        virtual void AddDrawable(const IDrawablePtr& drawable, int layer);
        virtual void RemoveDrawable(const IDrawablePtr& drawable);

        virtual void SetDrawableLayer(const IDrawablePtr& drawable, int new_layer);

        virtual mono::IEntityPtr FindEntityFromId(unsigned int id) const;
        virtual mono::IEntityPtr FindEntityFromPoint(const math::Vector& point) const;

    protected:

        virtual void Accept(IRenderer& renderer);
        virtual void Accept(mono::IUpdater& updater);
        virtual void DoPreAccept();
        
        std::vector<IEntityPtr> m_entities;
        std::vector<IUpdatablePtr> m_updatables;
        std::vector<std::pair<int, IDrawablePtr>> m_drawables;

        std::vector<IEntityPtr> m_entities_remove;
        std::vector<IUpdatablePtr> m_updatables_remove;
        std::vector<IDrawablePtr> m_drawables_remove;
    };
}
