
#pragma once

#include "ITool.h"
#include "Math/Vector2f.h"
#include <vector>

namespace editor
{
    class Editor;
    class PathEntity;

    class PathTool : public ITool
    {
    public:

        PathTool(Editor* editor);

        virtual void Begin();
        virtual void End();
        virtual bool IsActive() const;
        virtual void HandleContextMenu(int menu_index);
        virtual void HandleMouseDown(const math::Vector2f& world_pos, mono::IEntityPtr entity);
        virtual void HandleMouseUp(const math::Vector2f& world_pos);
        virtual void HandleMousePosition(const math::Vector2f& world_pos);

    private:

        Editor* m_editor;
        std::shared_ptr<editor::PathEntity> m_pathEntity;
        math::Vector2f m_mousePosition;
        bool m_active;

        //class Visualizer;
        //std::shared_ptr<Visualizer> m_visualizer;
    };
}
