
#pragma once

namespace mono
{
    enum class ParticleTransformSpace : int
    {
        WORLD,
        LOCAL
    };

    constexpr const char* particle_transform_space_strings[] = {
        "World",
        "Local",
    };

    inline const char* ParticleTransformSpaceToString(ParticleTransformSpace transform_space)
    {
        return particle_transform_space_strings[static_cast<int>(transform_space)];
    }

    enum class ParticleDrawLayer : int
    {
        PRE_GAMEOBJECTS,
        POST_GAMEOBJECTS
    };

    constexpr const char* particle_draw_layer_strings[] = {
        "Pre GameObjects",
        "Post GameObjects",
    };

    inline const char* ParticleDrawLayerToString(ParticleDrawLayer draw_layer)
    {
        return particle_draw_layer_strings[static_cast<int>(draw_layer)];
    }

    enum class EmitterType : int
    {
        CONTINOUS,
        BURST,
        BURST_REMOVE_ON_FINISH
    };

    constexpr const char* emitter_type_strings[] = {
        "Continous",
        "Burst",
        "Burst, Remove on Finish",
    };

    inline const char* EmitterTypeToString(EmitterType emitter_type)
    {
        return emitter_type_strings[static_cast<int>(emitter_type)];
    }

}
