
#pragma once

#include "IObjectProxy.h"
#include <memory>

namespace editor
{
    class Prefab;

    class PrefabProxy : public IObjectProxy
    {
    public:

        PrefabProxy(const std::shared_ptr<Prefab>& prefab);

        virtual unsigned int Id() const;
        virtual mono::IEntityPtr Entity();
        virtual void SetSelected(bool selected);
        virtual bool Intersects(const math::Vector& position) const;
        virtual std::vector<Grabber> GetGrabbers() const;
        virtual std::vector<SnapPoint> GetSnappers() const;
        virtual void UpdateUIContext(UIContext& context) const;

    private:
        std::shared_ptr<Prefab> m_prefab;
    };
}
