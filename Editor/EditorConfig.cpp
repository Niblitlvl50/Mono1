//
//  EditorConfig.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 24/07/16.
//
//

#include "EditorConfig.h"
#include "System/SysFile.h"
#include "nlohmann_json/json.hpp"

#include <cstdio>

namespace
{
    constexpr const char* camera_position = "camera_position";
    constexpr const char* camera_viewport = "camera_viewport";
}

bool editor::SaveConfig(const char* config_file, const editor::Config& config)
{
    nlohmann::json json;

    json[camera_position] = {
        config.cameraPosition.x,
        config.cameraPosition.y
    };
    
    json[camera_viewport] = {
        config.cameraViewport.mA.x,
        config.cameraViewport.mA.y,
        config.cameraViewport.mB.x,
        config.cameraViewport.mB.y
    };

    const std::string& serialized_config = json.dump(4);

    File::FilePtr file = File::CreateAsciiFile(config_file);
    std::fwrite(serialized_config.data(), serialized_config.length(), sizeof(char), file.get());

    return true;
}

bool editor::LoadConfig(const char* config_file, editor::Config& config)
{
    File::FilePtr file = File::OpenAsciiFile(config_file);
    if(!file)
        return false;

    std::vector<byte> file_data;
    File::FileRead(file, file_data);

    const nlohmann::json& json = nlohmann::json::parse(file_data);

    config.cameraPosition.x = json[camera_position][0];
    config.cameraPosition.y = json[camera_position][1];

    config.cameraViewport.mA.x = json[camera_viewport][0];
    config.cameraViewport.mA.y = json[camera_viewport][1];
    config.cameraViewport.mB.x = json[camera_viewport][2];
    config.cameraViewport.mB.y = json[camera_viewport][3];

    return true;
}
