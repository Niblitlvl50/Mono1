//
//  EditorConfig.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 24/07/16.
//
//

#include "EditorConfig.h"
#include "luatables++/luatables.h"

#include "System/SysFile.h"
#include <cstdio>

namespace
{
    constexpr const char* camera_position = "camera_position";
    constexpr const char* camera_viewport = "camera_viewport";
}

bool editor::SaveConfig(const char* config_file, const editor::Config& config)
{
    LuaTable table = LuaTable::fromLuaExpression(" return {} ");

    table[camera_position][1].set<float>(config.cameraPosition.x);
    table[camera_position][2].set<float>(config.cameraPosition.y);

    table[camera_viewport][1].set<float>(config.cameraViewport.mA.x);
    table[camera_viewport][2].set<float>(config.cameraViewport.mA.y);
    table[camera_viewport][3].set<float>(config.cameraViewport.mB.x);
    table[camera_viewport][4].set<float>(config.cameraViewport.mB.y);

    const std::string& serialized_config = table.serialize();

    File::FilePtr file = File::CreateAsciiFile(config_file);
    std::fwrite(serialized_config.data(), serialized_config.length(), sizeof(char), file.get());

    return true;
}

bool editor::LoadConfig(const char* config_file, editor::Config& config)
{
    LuaTable table = LuaTable::fromFile(config_file);

    config.cameraPosition.x = table[camera_position][1];
    config.cameraPosition.y = table[camera_position][2];

    config.cameraViewport.mA.x = table[camera_viewport][1];
    config.cameraViewport.mA.y = table[camera_viewport][2];
    config.cameraViewport.mB.x = table[camera_viewport][3];
    config.cameraViewport.mB.y = table[camera_viewport][4];

    return true;
}
