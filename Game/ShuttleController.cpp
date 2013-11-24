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

#include "PauseEvent.h"

using namespace game;

ShuttleController::ShuttleController(game::Shuttle* shuttle)
    : mShuttle(shuttle),
      mMouseDown(false)
{
    using namespace std::placeholders;

    const Event::MouseUpEventFunc mouseUpFunc = std::bind(&ShuttleController::OnMouseUp, this, _1);
    mMouseUpToken = mono::EventHandler::AddListener(mouseUpFunc);

    const Event::MouseDownEventFunc mouseDownFunc = std::bind(&ShuttleController::OnMouseDown, this, _1);
    mMouseDownToken = mono::EventHandler::AddListener(mouseDownFunc);
    
    const Event::MouseMotionEventFunc mouseMotionFunc = std::bind(&ShuttleController::OnMouseMotion, this, _1);
    mMouseMotionToken = mono::EventHandler::AddListener(mouseMotionFunc);
}

ShuttleController::~ShuttleController()
{
    mono::EventHandler::RemoveListener(mMouseDownToken);
    mono::EventHandler::RemoveListener(mMouseUpToken);
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
}

void ShuttleController::OnMouseUp(const Event::MouseUpEvent& event)
{
    mShuttle->mPhysicsObject.body->ResetForces();
    mMouseDown = false;
}

void ShuttleController::OnMouseMotion(const Event::MouseMotionEvent& event)
{
    mShuttle->mPhysicsObject.body->ResetForces();    
    
    math::Vector2f forceVector = mShuttle->Position() - math::Vector2f(event.mX, event.mY);
    math::Normalize(forceVector);
    
    mShuttle->mPhysicsObject.body->ApplyForce(forceVector * 200.0f, forceVector);
}

void ShuttleController::HandleOnShuttlePress()
{
    //mono::EventHandler::DispatchEvent(Event::PauseEvent());
    mShuttle->Fire();
}
