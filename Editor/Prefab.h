
#pragma once

#include "Entity/EntityBase.h"
#include "Rendering/RenderPtrFwd.h"
#include <string>
#include <vector>

namespace editor
{
    struct SnapPoint;

    class Prefab : public mono::EntityBase
    {
    public:

        Prefab(const std::string& name, const std::string& sprite_file, const std::vector<SnapPoint>& snap_points);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        void SetSelected(bool selected);
        const std::string& Name() const;
        const std::vector<SnapPoint>& SnapPoints() const;

    private:

        const std::string m_name;
        std::vector<SnapPoint> m_snap_points;
        bool m_selected;
        mono::ISpritePtr m_sprite;
    };
}
