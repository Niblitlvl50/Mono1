
#pragma once

#include "IGameSystem.h"
#include <vector>

namespace mono
{
    class SystemContext
    {
    public:

        SystemContext()
        { }

        ~SystemContext()
        {
            for(IGameSystem* system : m_systems)
                delete system;

            m_systems.clear();
        }

        template <typename T, typename ... A>
        inline T* CreateSystem(A&&... args)
        {
            T* new_system = new T(args...);
            m_systems.push_back(new_system);
            return new_system;
        }

        inline void DestroySystems()
        {
            for(IGameSystem* system : m_systems)
                system->Destroy();
        }

        template <typename T>
        inline T* GetSystem()
        {
            for(IGameSystem* game_system : m_systems)
            {
                T* system = dynamic_cast<T*>(game_system);
                if(system)
                    return system;
            }

            return nullptr;
        }

        inline void Update(const UpdateContext& update_context)
        {
            for(IGameSystem* game_system : m_systems)
                game_system->Update(update_context);
        }

        inline void Sync()
        {
            for(IGameSystem* game_system : m_systems)
                game_system->Sync();
        }

    private:
        std::vector<IGameSystem*> m_systems;
    };
}
