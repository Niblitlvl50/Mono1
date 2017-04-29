
#include "GravityUpdater.h"
#include "Zone/IPhysicsZone.h"
#include "Entity/IEntity.h"
#include "Math/Vector.h"
#include "Physics/IBody.h"

#include <cmath>

using namespace game;

GravityUpdater::GravityUpdater(mono::IPhysicsZone* zone, const mono::IEntityPtr& moon1, const mono::IEntityPtr& moon2)
    : mZone(zone),
        mMoon1(moon1),
        mMoon2(moon2),
        mElapsedTime(0)
{ }

void GravityUpdater::doUpdate(unsigned int delta)
{
    using namespace std::placeholders;

    mElapsedTime += delta;
    if(mElapsedTime < 16)
        return;
    
    mZone->ForEachBody(std::bind(&GravityUpdater::GravityFunc, this, _1));
    mElapsedTime = 0;
}

void GravityUpdater::GravityFunc(const mono::IBodyPtr& body)
{
    math::Vector impulse;
    
    math::Vector newPos = body->GetPosition() - mMoon1->Position();
    const float distance = math::Length(newPos);
    if(distance < 300.0f)
    {
        const float gravity = 1e4f;
        const float value = -gravity / (distance * std::sqrtf(distance));
    
        math::Normalize(newPos);
        newPos *= value;
        
        impulse += newPos;
    }
    
    math::Vector newPos2 = body->GetPosition() - mMoon2->Position();
    const float distance2 = math::Length(newPos2);
    if(distance2 < 200.0f)
    {
        const float gravity = 1e4f;
        const float value = -gravity / (distance2 * std::sqrtf(distance2));
        
        math::Normalize(newPos2);
        newPos2 *= value;
        
        impulse += newPos2;
    }
    
    body->ApplyImpulse(impulse, body->GetPosition());
}