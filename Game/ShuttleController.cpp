//
//  ShuttleController.cpp
//  Mono1
//
//  Created by Niblit on 2012-10-07.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "ShuttleController.h"
#include "Shuttle.h"
#include "EventHandler.h"
#include "EventFuncFwd.h"
#include "MouseUpEvent.h"
#include "MouseDownEvent.h"
#include "MouseMotionEvent.h"
#include "MultiGestureEvent.h"
#include "KeyDownEvent.h"
#include "CMIBody.h"
#include "SysKeycodes.h"
#include "WeaponTypes.h"
#include "MathFunctions.h"

#include <cmath>


using namespace game;

ShuttleController::ShuttleController(game::Shuttle* shuttle, mono::EventHandler& eventHandler)
    : mShuttle(shuttle),
      mEventHandler(eventHandler),
      mMouseDown(false)
{
    using namespace std::placeholders;

    const Event::MouseUpEventFunc mouseUpFunc = std::bind(&ShuttleController::OnMouseUp, this, _1);
    mMouseUpToken = mEventHandler.AddListener(mouseUpFunc);

    const Event::MouseDownEventFunc mouseDownFunc = std::bind(&ShuttleController::OnMouseDown, this, _1);
    mMouseDownToken = mEventHandler.AddListener(mouseDownFunc);
    
    const Event::MouseMotionEventFunc mouseMotionFunc = std::bind(&ShuttleController::OnMouseMotion, this, _1);
    mMouseMotionToken = mEventHandler.AddListener(mouseMotionFunc);

    const Event::KeyDownEventFunc keyDownFunc = std::bind(&ShuttleController::OnKeyDown, this, _1);
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

void ShuttleController::OnMouseDown(const Event::MouseDownEvent& event)
{
    const bool result = math::PointInsideQuad(math::Vector2f(event.worldX, event.worldY), mShuttle->BoundingBox());
    if(result)
    {
        HandleOnShuttlePress();
        return;
    }
    
    mMouseDown = true;
    mMouseDownPosition = math::Vector2f(event.screenX, event.screenY);
    mShuttle->StartThrusting();
}

void ShuttleController::OnMouseUp(const Event::MouseUpEvent& event)
{
    mShuttle->mPhysicsObject.body->ResetForces();
    mShuttle->StopThrusting();
    mMouseDown = false;
}

void ShuttleController::OnMouseMotion(const Event::MouseMotionEvent& event)
{
    if(!mMouseDown)
        return;

    // Reset forces first.
    mShuttle->mPhysicsObject.body->ResetForces();

    const math::Vector2f current(event.worldX, event.worldY);
    math::Vector2f force = current - mShuttle->mPosition;
    math::Normalize(force);

    mShuttle->ApplyImpulse(force * 100);
}

void ShuttleController::OnMultiGesture(const Event::MultiGestureEvent& event)
{
    const float force = event.theta * 1000;
    mShuttle->ApplyRotationForce(force);
}

void ShuttleController::OnKeyDown(const Event::KeyDownEvent& event)
{
    if(event.key == Key::ONE)
        mShuttle->SelectWeapon(WeaponType::STANDARD);
    else if(event.key == Key::TWO)
        mShuttle->SelectWeapon(WeaponType::ROCKET);
}

void ShuttleController::HandleOnShuttlePress()
{
    mShuttle->Fire();
}
