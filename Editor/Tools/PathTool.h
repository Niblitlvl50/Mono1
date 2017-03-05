
#pragma once

#include "ITool.h"
#include "Math/Vector.h"
#include <vector>

namespace editor
{
    class Editor;

    class PathTool : public ITool
    {
    public:

        PathTool(Editor* editor);

        virtual void Begin();
        virtual void End();
        virtual bool IsActive() const;
        virtual void HandleContextMenu(int menu_index);
        virtual void HandleMouseDown(const math::Vector& world_pos, mono::IEntityPtr entity);
        virtual void HandleMouseUp(const math::Vector& world_pos);
        virtual void HandleMousePosition(const math::Vector& world_pos);

    private:

        Editor* m_editor;
        math::Vector m_mousePosition;
        std::vector<math::Vector> m_points;

        class Visualizer;
        std::shared_ptr<Visualizer> m_visualizer;
    };
}
