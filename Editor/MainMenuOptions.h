//
//  MainMenuOptions.h
//  MonoiOS
//
//  Created by Niklas Damberg on 24/07/16.
//
//

#pragma once

namespace editor
{
    enum class EditorMenuOptions
    {
        OPEN,
        SAVE,
        EXPORT
    };

    enum class ToolsMenuOptions : int
    {
        POLYGON_TOOL,
        POLYGON_BRUSH_TOOL,
        PATH_TOOL,
        TRANSLATE_TOOL,
        ROTATE_TOOL
    };
}
