
#include "WriteSpriteFile.h"
#include "System/SysFile.h"
#include "Sprite/AnimationSequence.h"
#include "nlohmann_json/json.hpp"

void animator::WriteSpriteFile(const char* sprite_file, const std::vector<mono::AnimationSequence>& animations)
{
    File::FilePtr input_file = File::OpenAsciiFile(sprite_file);
    if(!input_file)
        return;

    std::vector<byte> file_data;
    File::FileRead(input_file, file_data);

    input_file = nullptr;

    nlohmann::json json = nlohmann::json::parse(file_data);
    nlohmann::json& json_animations = json["animations"];
    json_animations.clear();

    for(const mono::AnimationSequence& animation : animations)
    {
        const bool loop = animation.IsLooping();

        std::vector<int> values;

        for(const mono::Frame& frame : animation.GetFrames())
        {
            values.push_back(frame.frame);
            values.push_back(frame.duration);
        }

        nlohmann::json object;
        object["loop"] = animation.IsLooping();
        object["frames"] = values;

        json_animations.push_back(object);
    }

    const std::string& serialized_sprite = json.dump(4);
    File::FilePtr output_file = File::CreateAsciiFile(sprite_file);
    std::fwrite(serialized_sprite.data(), serialized_sprite.length(), sizeof(char), output_file.get());
}