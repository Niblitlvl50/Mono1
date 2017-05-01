
#pragma once

#include "Rendering/IDrawable.h"
#include "Rendering/RenderPtrFwd.h"
#include <vector>

namespace editor
{
    class GridVisualizer : public mono::IDrawable
    {
    public:

        GridVisualizer(const mono::ICameraPtr& camera);

        virtual void doDraw(mono::IRenderer& renderer) const;
        virtual math::Quad BoundingBox() const;

    private:
        
        const mono::ICameraPtr& m_camera;
        std::vector<math::Vector> m_gridVertices;
    };
}
