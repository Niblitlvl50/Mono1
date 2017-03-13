
#pragma once

#include "IObjectProxy.h"
#include <memory>

namespace editor
{
    class SpriteEntity;

    class EntityProxy : public editor::IObjectProxy
    {
    public:

        EntityProxy(const std::shared_ptr<SpriteEntity>& entity);

        virtual uint Id() const;
        virtual mono::IEntityPtr Entity();
        virtual void SetSelected(bool selected);
        virtual bool Intersects(const math::Vector& position) const;
        virtual std::vector<Grabber> GetGrabbers() const;
        virtual std::vector<SnapPoint> GetSnappers() const;
        virtual void UpdateUIContext(UIContext& context) const;

    private:
        std::shared_ptr<SpriteEntity> m_entity;
    };
}
