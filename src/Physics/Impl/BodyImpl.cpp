
#include "BodyImpl.h"
#include "Math/Vector.h"
#include "Util/Algorithm.h"

#include "chipmunk/chipmunk.h"

using namespace cm;

BodyImpl::BodyImpl(uint32_t body_id, cpBody* body)
    : m_body_id(body_id)
    , m_body_handle(body)
    , m_auto_calculate_moment(true)
    , m_prevent_rotation(false)
    , m_custom_damping(1.0f)
    , m_material(0)
{ }

uint32_t BodyImpl::GetId() const
{
    return m_body_id;
}

void BodyImpl::SetType(mono::BodyType type)
{
    cpBodySetType(m_body_handle, static_cast<cpBodyType>(type));
}

mono::BodyType BodyImpl::GetType() const
{
    return mono::BodyType(cpBodyGetType(m_body_handle));
}

bool BodyImpl::IsSleeping() const
{
    return cpBodyIsSleeping(m_body_handle);
}

void BodyImpl::SetMass(float mass) 
{
    cpBodySetMass(m_body_handle, mass);
}

float BodyImpl::GetMass() const 
{
    return cpBodyGetMass(m_body_handle);
}

void BodyImpl::SetAngle(float angle) 
{
    cpBodySetAngle(m_body_handle, angle);
}

float BodyImpl::GetAngle() const 
{
    return cpBodyGetAngle(m_body_handle);
}

void BodyImpl::SetPreventRotation(bool prevent_rotation)
{
    m_prevent_rotation = prevent_rotation;
}

bool BodyImpl::PreventRotation() const
{
    return m_prevent_rotation;
}

void BodyImpl::SetMoment(float moment)
{
    const float new_moment = m_prevent_rotation ? math::INF : moment;
    cpBodySetMoment(m_body_handle, new_moment);
}

float BodyImpl::GetMoment() const 
{
    return cpBodyGetMoment(m_body_handle);
}

void BodyImpl::SetCustomDamping(float damping)
{
    m_custom_damping = damping;

    const cpBodyVelocityFunc custom_damping_func = [](cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt)
    {
        const mono::IBody* body_impl = (mono::IBody*)cpBodyGetUserData(body);
        const float custom_damping = body_impl->GetCustomDamping();

        // Use default but set no damping.
        cpBodyUpdateVelocity(body, gravity, custom_damping, dt);
    };

    cpBodySetVelocityUpdateFunc(m_body_handle, custom_damping_func);
}

float BodyImpl::GetCustomDamping() const
{
    return m_custom_damping;
}

void BodyImpl::ClearCustomDamping()
{
    cpBodySetVelocityUpdateFunc(m_body_handle, cpBodyUpdateVelocity);
}

void BodyImpl::SetNoDamping() 
{
    SetCustomDamping(1.0f);
}

void BodyImpl::SetAutoCalculateMoment(bool calculate_moment)
{
    m_auto_calculate_moment = calculate_moment;
}

bool BodyImpl::AutoCalculateMoment() const
{
    return m_auto_calculate_moment;
}

void BodyImpl::SetPosition(const math::Vector& position) 
{
    cpBodySetPosition(m_body_handle, cpv(position.x, position.y));
}

math::Vector BodyImpl::GetPosition() const 
{
    const cpVect& position = cpBodyGetPosition(m_body_handle);
    return math::Vector(position.x, position.y);
}

math::Vector BodyImpl::LocalToWorld(const math::Vector& local_point) const
{
    const cpVect& world_point = cpBodyLocalToWorld(m_body_handle, cpv(local_point.x, local_point.y));
    return math::Vector(world_point.x, world_point.y);
}

math::Vector BodyImpl::WorldToLocal(const math::Vector& world_point) const
{
    const cpVect& local_point = cpBodyWorldToLocal(m_body_handle, cpv(world_point.x, world_point.y));
    return math::Vector(local_point.x, local_point.y);
}

void BodyImpl::ApplyForce(const math::Vector& force, const math::Vector& offset) 
{
    cpBodyApplyForceAtWorldPoint(m_body_handle, cpv(force.x, force.y), cpv(offset.x, offset.y));
}

void BodyImpl::ApplyLocalForce(const math::Vector& force, const math::Vector& offset) 
{
    cpBodyApplyForceAtLocalPoint(m_body_handle, cpv(force.x, force.y), cpv(offset.x, offset.y));
}

void BodyImpl::ApplyImpulse(const math::Vector& impulse, const math::Vector& offset) 
{
    cpBodyApplyImpulseAtWorldPoint(m_body_handle, cpv(impulse.x, impulse.y), cpv(offset.x, offset.y));
}

void BodyImpl::ApplyLocalImpulse(const math::Vector& impulse, const math::Vector& offset) 
{
    cpBodyApplyImpulseAtLocalPoint(m_body_handle, cpv(impulse.x, impulse.y), cpv(offset.x, offset.y));
}

math::Vector BodyImpl::GetForce() const
{
    const cpVect& force = cpBodyGetForce(m_body_handle);
    return math::Vector(force.x, force.y);
}

void BodyImpl::SetVelocity(const math::Vector& velocity) 
{
    cpBodySetVelocity(m_body_handle, cpv(velocity.x, velocity.y));
}

math::Vector BodyImpl::GetVelocity() const
{
    const cpVect& velocity = cpBodyGetVelocity(m_body_handle);
    return math::Vector(velocity.x, velocity.y);
}

void BodyImpl::ResetForces() 
{
    cpBodySetForce(m_body_handle, cpvzero);
}

void BodyImpl::AddCollisionHandler(mono::ICollisionHandler* collision_handler) 
{
    m_collision_handlers.push_back(collision_handler);
}

void BodyImpl::RemoveCollisionHandler(mono::ICollisionHandler* handler)
{
    mono::remove(m_collision_handlers, handler);
}

mono::CollisionResolve BodyImpl::OnCollideWith(
    mono::IBody* body, const math::Vector& collision_point, const math::Vector& collision_normal, uint32_t categories) 
{
    mono::CollisionResolve resolve_result = mono::CollisionResolve::NORMAL;

    for(mono::ICollisionHandler* handler : m_collision_handlers)
    {
        const mono::CollisionResolve resolve = handler->OnCollideWith(body, collision_point, collision_normal, categories);
        resolve_result = std::max(resolve_result, resolve);
    }

    return resolve_result;
}

void BodyImpl::OnSeparateFrom(IBody* body)
{
    for(mono::ICollisionHandler* handler : m_collision_handlers)
        handler->OnSeparateFrom(body);
}

uint32_t BodyImpl::GetMaterial() const
{
    return m_material;
}

void BodyImpl::SetMaterial(uint32_t material)
{
    m_material = material;
}

cpBody* BodyImpl::Handle() const
{
    return m_body_handle;
}

void BodyImpl::ClearCollisionHandlers()
{
    m_collision_handlers.clear();
}
