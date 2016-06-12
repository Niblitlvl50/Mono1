//
//  GameContext.h
//  MonoiOS
//
//  Created by Niklas Damberg on 10/06/16.
//
//

#include "MonoFwd.h"

namespace game
{
    class ISpawnSystem
    {
    public:

        virtual ~ISpawnSystem()
        { }
    };

    class IGameObjectSystem
    {
    public:

        virtual ~IGameObjectSystem()
        { }

    };

    class ILevelSystem
    {
    public:

        virtual ~ILevelSystem()
        { }
    };

    struct GameContext
    {
        ISpawnSystem* spaw_system;
        IGameObjectSystem* gameobject_system;
        ILevelSystem* level_system;
        mono::EventHandler* event_handler;
    } context;
}