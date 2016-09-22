//
//  ShuttleController.cpp
//  Mono1
//
//  Created by Niblit on 2012-10-07.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "ShuttleController.h"
#include "Shuttle.h"
#include "EventHandler/EventHandler.h"
#include "Events/EventFuncFwd.h"
#include "Events/MouseUpEvent.h"
#include "Events/MouseDownEvent.h"
#include "Events/MouseMotionEvent.h"
#include "Events/MultiGestureEvent.h"
#include "Events/KeyDownEvent.h"
#include "System/SysKeycodes.h"
#include "Weapons/WeaponTypes.h"
#include "Math/MathFunctions.h"


using namespace game;

ShuttleController::ShuttleController(game::Shuttle* shuttle, mono::EventHandler& eventHandler)
    : mShuttle(shuttle),
      mEventHandler(eventHandler),
      mMouseDown(false)
{
    using namespace std::placeholders;

    const event::MouseUpEventFunc mouseUpFunc = std::bind(&ShuttleController::OnMouseUp, this, _1);
    mMouseUpToken = mEventHandler.AddListener(mouseUpFunc);

    const event::MouseDownEventFunc mouseDownFunc = std::bind(&ShuttleController::OnMouseDown, this, _1);
    mMouseDownToken = mEventHandler.AddListener(mouseDownFunc);
    
    const event::MouseMotionEventFunc mouseMotionFunc = std::bind(&ShuttleController::OnMouseMotion, this, _1);
    mMouseMotionToken = mEventHandler.AddListener(mouseMotionFunc);

    const event::KeyDownEventFunc keyDownFunc = std::bind(&ShuttleController::OnKeyDown, this, _1);
    mKeyDownToken = mEventHandler.AddListener(keyDownFunc);

    //const Event::MultiGestureEventFunc multiGestureFunc = std::bind(&ShuttleController::OnMultiGesture, this, _1);
    //mMultiGestureToken = mEventHandler.AddListener(multiGestureFunc);
}

ShuttleController::~ShuttleController()
{
    mEventHandler.RemoveListener(mMouseUpToken);
    mEventHandler.RemoveListener(mMouseDownToken);
    mEventHandler.RemoveListener(mMouseMotionToken);
    //mEventHandler.RemoveListener(mMultiGestureToken);
}

bool ShuttleController::OnMouseDown(const event::MouseDownEvent& event)
{
    mShuttle->Fire();

    mMouseDownPosition = math::Vector2f(event.screenX, -event.screenY);
    mShuttle->StartThrusting();
    mMouseDown = true;

    return true;
}

bool ShuttleController::OnMouseUp(const event::MouseUpEvent& event)
{
    mShuttle->StopFire();
    
    mShuttle->mPhysicsObject.body->ResetForces();
    mShuttle->StopThrusting();
    mMouseDown = false;

    return true;
}

bool ShuttleController::OnMouseMotion(const event::MouseMotionEvent& event)
{
    if(!mMouseDown)
        return false;

    // Reset forces first.
    mShuttle->mPhysicsObject.body->ResetForces();

    const math::Vector2f current(event.screenX, -event.screenY);

    math::Vector2f force = current - mMouseDownPosition;
    math::Normalize(force);

    mShuttle->ApplyImpulse(force * 100);

    mMouseDownPosition = current;

    return false;
}

bool ShuttleController::OnMultiGesture(const event::MultiGestureEvent& event)
{
    const float force = event.theta * 1000;
    mShuttle->ApplyRotationForce(force);

    return true;
}

bool ShuttleController::OnKeyDown(const event::KeyDownEvent& event)
{
    if(event.key != Key::ONE && event.key != Key::TWO && event.key != Key::THREE)
        return false;

    if(event.key == Key::ONE)
       mShuttle->SelectWeapon(WeaponType::STANDARD);
    else if(event.key == Key::TWO)
        mShuttle->SelectWeapon(WeaponType::ROCKET);
    else
        mShuttle->SelectWeapon(WeaponType::CACOPLASMA);

    return true;
}
