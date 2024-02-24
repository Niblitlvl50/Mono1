
#pragma once

#include "Physics/IBody.h"
#include <vector>
#include <cstdint>

namespace cm
{
    class BodyImpl : public mono::IBody
    {
    public:
    
        BodyImpl(uint32_t body_id, cpBody* body);

        uint32_t GetId() const override;
        void SetType(mono::BodyType type) override;
        mono::BodyType GetType() const override;
        bool IsSleeping() const override;
        void SetMass(float mass) override;
        float GetMass() const override;
        void SetAngle(float angle) override;
        float GetAngle() const override;
        void SetPreventRotation(bool prevent_rotation) override;
        bool PreventRotation() const override;
        void SetMoment(float moment) override;
        float GetMoment() const override;
        void SetAutoCalculateMoment(bool calculate_moment) override;
        bool AutoCalculateMoment() const override;
        void SetPosition(const math::Vector& position) override;
        math::Vector GetPosition() const override;
        math::Vector LocalToWorld(const math::Vector& local_point) const override;
        math::Vector WorldToLocal(const math::Vector& world_point) const override;
        void ApplyForce(const math::Vector& force, const math::Vector& offset) override;
        void ApplyLocalForce(const math::Vector& force, const math::Vector& offset) override;
        void ApplyImpulse(const math::Vector& impulse, const math::Vector& offset) override;
        void ApplyLocalImpulse(const math::Vector& impulse, const math::Vector& offset) override;
        math::Vector GetForce() const override;
        void SetVelocity(const math::Vector& velocity) override;
        math::Vector GetVelocity() const override;
        void ResetForces() override;
        void AddCollisionHandler(mono::ICollisionHandler* collision_handler) override;
        void RemoveCollisionHandler(mono::ICollisionHandler* handler) override;
        mono::CollisionResolve OnCollideWith(
            mono::IBody* body, const math::Vector& collision_point, const math::Vector& collision_normal, uint32_t categories) override;
        void OnSeparateFrom(mono::IBody* body) override;
        void SetNoDamping() override;
        cpBody* Handle() const override;

        void ClearCollisionHandlers();

    private:

        const uint32_t m_body_id;
        cpBody* m_body_handle;
        bool m_auto_calculate_moment;
        bool m_prevent_rotation;
        std::vector<mono::ICollisionHandler*> m_collision_handlers;
    };
}
