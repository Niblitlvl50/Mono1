
#pragma once

#include "IObjectProxy.h"
#include <memory>

namespace editor
{
    class PathEntity;
    class Editor;

    class PathProxy : public editor::IObjectProxy
    {
    public:

        PathProxy(const std::shared_ptr<PathEntity>& path, Editor* editor);

        virtual unsigned int Id() const;
        virtual mono::IEntityPtr Entity();
        virtual void SetSelected(bool selected);
        virtual bool Intersects(const math::Vector& position) const;
        virtual std::vector<Grabber> GetGrabbers() const;
        virtual std::vector<SnapPoint> GetSnappers() const;
        virtual void UpdateUIContext(UIContext& context) const;

    private:
        std::shared_ptr<PathEntity> m_path;
        Editor* m_editor;
    };
}
