
#pragma once

#include "IGameSystem.h"
#include "Math/Matrix.h"
#include "Math/Quad.h"
#include <vector>

namespace mono
{
    class TransformSystem : public mono::IGameSystem
    {
    public:

        struct Component
        {
            math::Matrix transform;
            math::Quad bounding_box;
            uint32_t parent;
        };

        TransformSystem(size_t n_components);

        math::Matrix GetWorld(uint32_t id) const;
        const math::Matrix& GetTransform(uint32_t id) const;
        math::Matrix& GetTransform(uint32_t id);

        math::Quad GetWorldBoundingBox(uint32_t id) const;
        const math::Quad& GetBoundingBox(uint32_t id) const;
        math::Quad& GetBoundingBox(uint32_t id);

        uint32_t GetParent(uint32_t id) const;
        void ChildTransform(uint32_t id, uint32_t parent);
        void UnchildTransform(uint32_t id);

        uint32_t Id() const override;
        const char* Name() const override;
        uint32_t Capacity() const override;
        void Update(const UpdateContext& update_context) override;

        template <typename T>
        inline void ForEachTransform(T&& func)
        {
            for(uint32_t index = 0; index < m_transforms.size(); ++index)
            {
                Component& component = m_transforms[index];
                func(component.transform, index);
            }
        }

        template <typename T>
        inline void ForEachComponent(T&& func) const
        {
            for(uint32_t index = 0; index < m_transforms.size(); ++index)
                func(m_transforms[index], index);
        }

    private:


        std::vector<Component> m_transforms;
    };
}
