
#pragma once

#include "IObjectProxy.h"

namespace editor
{
    class Editor;

    class PolygonProxy : public editor::IObjectProxy
    {
    public:

        PolygonProxy(uint id, Editor* editor);

        virtual uint Id() const;
        virtual void SetSelected(bool selected);
        virtual bool Intersects(const math::Vector& position) const;
        virtual std::vector<Grabber> GetGrabbers() const;

    private:
        const uint m_id;
        Editor* m_editor;
    };
}
