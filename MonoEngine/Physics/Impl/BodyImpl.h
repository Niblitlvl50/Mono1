
#pragma once

#include "Physics/IBody.h"

namespace cm
{
    class BodyImpl : public mono::IBody
    {
    public:
    
        BodyImpl(cpBody* body);

        bool IsStatic() const override;
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
        void ResetForces() override;
        void SetCollisionHandler(mono::ICollisionHandler* collision_handler) override;
        void OnCollideWith(const mono::IBodyPtr& body, unsigned int categories) override;
        void SetNoDamping() override;
        cpBody* Handle() override;
        
    private:
        cpBody* m_body_handle;
        mono::ICollisionHandler* m_collision_handler;
    };
}
