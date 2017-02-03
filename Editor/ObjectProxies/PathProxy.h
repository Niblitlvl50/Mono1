
#pragma once

#include "IObjectProxy.h"

namespace editor
{
    class Editor;

    class PathProxy : public editor::IObjectProxy
    {
    public:
    
        PathProxy(uint id, Editor* editor);

        virtual uint Id() const;
        virtual void SetSelected(bool selected);
        virtual bool Intersects(const math::Vector& position) const;
        virtual std::vector<Grabber> GetGrabbers() const;

        const uint m_id;
        Editor* m_editor;
    };
}
