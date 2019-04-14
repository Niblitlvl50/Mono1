
#pragma once

#include "IGameSystem.h"
#include "Entity.h"
#include <vector>
#include <string>

namespace mono
{
    class EntitySystem : public mono::IGameSystem
    {
    public:

        EntitySystem(size_t n_entities);

        Entity& AllocateEntity();
        void ReleaseEntity(uint32_t entity_id);

        Entity& GetEntity(uint32_t entity_id);

        void SetProperty(Entity id, uint32_t property);
        bool HasProperty(Entity id, uint32_t property) const;

        void SetName(uint32_t entity_id, const std::string& name);
        const std::string& GetName(uint32_t entity_id) const;

        template <typename T>
        inline void ForEach(T&& functor)
        {
            for(size_t index = 0; index < m_entities.size(); ++index)
            {
                const Entity& entity = m_entities[index];
                if(entity.id != INVALID_ID)
                    functor(entity);
            }
        }

        uint32_t Id() const override;
        const char* Name() const override;
        uint32_t Capacity() const override;
        void Update(uint32_t delta_ms) override;

    private:
        std::vector<Entity> m_entities;
        std::vector<bool> m_alive;
        std::vector<uint32_t> m_free_indices;
        std::vector<std::string> m_debug_names;
    };
}
