
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
            for(auto it = m_systems.rbegin(), end = m_systems.rend(); it != end; ++it)
            {
                delete (*it);
                *it = nullptr;
            }

            m_systems.clear();
        }

        template <typename T, typename ... A>
        inline T* CreateSystem(A&&... args)
        {
            T* new_system = new T(args...);
            m_systems.push_back(new_system);
            return new_system;
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
            const bool paused = update_context.paused;

            for(IGameSystem* game_system : m_systems)
            {
                if(!paused || (paused && game_system->UpdateInPause()))
                    game_system->Update(update_context);
            }
        }

        inline void PostUpdate(const UpdateContext& update_context)
        {
            const bool paused = update_context.paused;

            for(IGameSystem* game_system : m_systems)
            {
                if(!paused || (paused && game_system->UpdateInPause()))
                    game_system->PostUpdate();
            }
        }

        inline void BeginSystems()
        {
            for(IGameSystem* system : m_systems)
                system->Begin();
        }

        // Should sync be done in reverse order?
        inline void SyncSystems(bool paused = false)
        {
            /*
            for(IGameSystem* game_system : m_systems)
            {
                if(!paused || (paused && game_system->UpdateInPause()))
                game_system->Sync();
            }
            */

            for(auto it = m_systems.rbegin(), end = m_systems.rend(); it != end; ++it)
            {
                IGameSystem* game_system = *it;
                if(!paused || (paused && game_system->UpdateInPause()))
                    game_system->Sync();
            }
        }

        inline void ResetSystems()
        {
            for(auto it = m_systems.rbegin(), end = m_systems.rend(); it != end; ++it)
                (*it)->Reset();
        }

        inline void DestroySystems()
        {
            for(auto it = m_systems.rbegin(), end = m_systems.rend(); it != end; ++it)
                (*it)->Destroy();
        }

    private:
        std::vector<IGameSystem*> m_systems;
    };
}
