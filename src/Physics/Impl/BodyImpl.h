
#pragma once

#include "Physics/IBody.h"
#include <vector>

namespace cm
{
    class BodyImpl : public mono::IBody
    {
    public:
    
        BodyImpl(cpBody* body);

        void SetType(mono::BodyType type) override;
        mono::BodyType GetType() const override;
        bool IsSleeping() const override;
        void SetMass(float mass) override;
        float GetMass() const override;
        void SetAngle(float angle) override;
        float GetAngle() const override;
        void SetMoment(float moment) override;
        float GetMoment() const override;
        void SetPosition(const math::Vector& position) override;
        math::Vector GetPosition() const override;
        void ApplyForce(const math::Vector& force, const math::Vector& offset) override;
        void ApplyLocalForce(const math::Vector& force, const math::Vector& offset) override;
        void ApplyImpulse(const math::Vector& impulse, const math::Vector& offset) override;
        void ApplyLocalImpulse(const math::Vector& impulse, const math::Vector& offset) override;
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
        cpBody* m_body_handle;
        std::vector<mono::ICollisionHandler*> m_collision_handlers;
    };
}
