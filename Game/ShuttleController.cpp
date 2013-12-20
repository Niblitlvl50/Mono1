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
#include "CMIBody.h"

#include "MathFunctions.h"

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
}

ShuttleController::~ShuttleController()
{
    mEventHandler.RemoveListener(mMouseUpToken);
    mEventHandler.RemoveListener(mMouseDownToken);
    mEventHandler.RemoveListener(mMouseMotionToken);
}

void ShuttleController::OnMouseDown(const Event::MouseDownEvent& event)
{
    const bool result = math::PointInsideQuad(math::Vector2f(event.mX, event.mY), mShuttle->BoundingBox());
    if(result)
    {
        HandleOnShuttlePress();
        return;
    }
    
    mMouseDown = true;
    
    math::Vector2f forceVector = mShuttle->Position() - math::Vector2f(event.mX, event.mY);
    math::Normalize(forceVector);
    
    mShuttle->mPhysicsObject.body->ApplyForce(forceVector * 200.0f, forceVector);
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
    
    mShuttle->mPhysicsObject.body->ResetForces();    
    
    math::Vector2f forceVector = mShuttle->Position() - math::Vector2f(event.mX, event.mY);
    math::Normalize(forceVector);
    
    mShuttle->mPhysicsObject.body->ApplyForce(forceVector * 200.0f, forceVector);
}

void ShuttleController::HandleOnShuttlePress()
{
    mShuttle->Fire();
}
