
#pragma once

#include "IObjectProxy.h"
#include <memory>

namespace editor
{
    class PolygonEntity;

    class PolygonProxy : public editor::IObjectProxy
    {
    public:

        PolygonProxy(const std::shared_ptr<PolygonEntity>& polygon);

        virtual unsigned int Id() const;
        virtual mono::IEntityPtr Entity();
        virtual void SetSelected(bool selected);
        virtual bool Intersects(const math::Vector& position) const;
        virtual std::vector<Grabber> GetGrabbers() const;
        virtual std::vector<SnapPoint> GetSnappers() const;
        virtual void UpdateUIContext(UIContext& context) const;

    private:

        std::shared_ptr<PolygonEntity> m_polygon;
    };
}
