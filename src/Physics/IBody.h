
#pragma once

#include "Math/MathFwd.h"
#include <cstdint>

struct cpBody;

namespace mono
{
    class IBody;

    enum class BodyType : int
    {
        DYNAMIC,
        KINEMATIC,
        STATIC
    };

    enum class CollisionResolve
    {
        NORMAL,
        IGNORE
    };

    class ICollisionHandler
    {
    public:

        virtual ~ICollisionHandler() = default;
        virtual mono::CollisionResolve OnCollideWith(
            mono::IBody* body, const math::Vector& collision_point, const math::Vector& collision_normal, uint32_t categories) = 0;
        virtual void OnSeparateFrom(mono::IBody* body) = 0;
    };

    class IBody
    {
    public:

        virtual ~IBody() = default;
        
        virtual void SetType(BodyType type) = 0;
        virtual BodyType GetType() const = 0;
        virtual bool IsSleeping() const = 0;
        
        //! Sets the bodys mass
        //! @param mass The new mass
        virtual void SetMass(float mass) = 0;
        
        //! Get this bodys mass
        //! @return float The mass
        virtual float GetMass() const = 0;
        
        //! Sets the angle in radians
        //! @param angle New angle
        virtual void SetAngle(float angle) = 0;
        
        //! Gets the angle in radians
        //! @return float Angle
        virtual float GetAngle() const = 0;
        
        //! Sets the moment(inertia)
        //! @param moment Moment
        virtual void SetMoment(float moment) = 0;
        
        //! Gets the moment(inertia) of the body
        //! @return float moment
        virtual float GetMoment() const = 0;
        
        //! Set the position of the body
        //! @param position A new position
        virtual void SetPosition(const math::Vector& position) = 0;
        
        //! Gets the bodys position
        //! @return math::Vector The position
        virtual math::Vector GetPosition() const = 0;

        //! Apply a force to the body
        //! @param force The force
        //! @param offset From where to apply it
        virtual void ApplyForce(const math::Vector& force, const math::Vector& offset) = 0;

        //! Apply a local force to the body
        //! @param force The force
        //! @param offset From where to apply it
        virtual void ApplyLocalForce(const math::Vector& force, const math::Vector& offset) = 0;
        
        //! Apply an impulse to the body
        //! @param impulse The impulse
        //! @param offset From where to apply it
        virtual void ApplyImpulse(const math::Vector& impulse, const math::Vector& offset) = 0;

        //! Apply a local impulse to the body
        //! @param impulse The impulse
        //! @param offset From where to apply it
        virtual void ApplyLocalImpulse(const math::Vector& impulse, const math::Vector& offset) = 0;

        virtual math::Vector GetForce() const = 0;

        //! Sets a velocity to the body
        //! @param velocity A velocity vector
        virtual void SetVelocity(const math::Vector& velocity) = 0;
        virtual math::Vector GetVelocity() const = 0;

        //! Resets all the forces that are working on the body
        virtual void ResetForces() = 0;
        
        //! Sets a collision handler to the body
        //! @param handler A pointer to a collision handler
        virtual void AddCollisionHandler(mono::ICollisionHandler* handler) = 0;
        virtual void RemoveCollisionHandler(mono::ICollisionHandler* handler) = 0;
        
        //! Called by the framework when a collision occures
        //! @param body The other body of the collision
        virtual mono::CollisionResolve OnCollideWith(
            IBody* body, const math::Vector& collision_point, const math::Vector& collision_normal, uint32_t categories) = 0;

        virtual void OnSeparateFrom(mono::IBody* body) = 0;

        //! Disable damping on this body, will cause no slowdowns. 
        virtual void SetNoDamping() = 0;
        
        //! Gets the underlying cpBody object, dont use this one
        //! @return cpBody* A pointer to a cpBody struct
        virtual cpBody* Handle() const = 0;
    };
}
