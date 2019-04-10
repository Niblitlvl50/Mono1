
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
        void Update(uint32_t delta_ms) override;

    private:

        struct Component
        {
            math::Matrix transform;
            math::Quad bounding_box;
            uint32_t parent;
        };

        std::vector<Component> m_transforms;
    };
}
