//
//  GridVisualizer.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 12/08/16.
//
//

#pragma once

#include "IDrawable.h"
#include "MonoPtrFwd.h"
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
        std::vector<math::Vector2f> m_gridVertices;
    };
}
