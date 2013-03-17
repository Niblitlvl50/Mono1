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
#include "MouseUpEvent.h"
#include "MouseDownEvent.h"
#include "CMIBody.h"

#include "MathFunctions.h"

#include "PauseEvent.h"

using namespace game;

ShuttleController::ShuttleController(game::Shuttle* shuttle)
    : mShuttle(shuttle)
{
    using namespace std::tr1::placeholders;

    const Event::MouseUpEventFunc mouseUpFunc = std::tr1::bind(&ShuttleController::OnMouseUp, this, _1);
    mMouseUpToken = mono::EventHandler::AddListener(mouseUpFunc);

    const Event::MouseDownEventFunc mouseDownFunc = std::tr1::bind(&ShuttleController::OnMouseDown, this, _1);
    mMouseDownToken = mono::EventHandler::AddListener(mouseDownFunc);
}

ShuttleController::~ShuttleController()
{
    mono::EventHandler::RemoveListener(mMouseDownToken);
    mono::EventHandler::RemoveListener(mMouseUpToken);
}

void ShuttleController::OnMouseDown(const Event::MouseDownEvent& event)
{
    const bool result1 = Math::PointInsideQuad(Math::Vector2f(event.mX, event.mY), mShuttle->BoundingBox());
    if(result1)
        HandleOnShuttlePress();
        
    Math::Vector2f result = mShuttle->Position() - Math::Vector2f(event.mX, event.mY);
    Math::Normalize(result);
    
    mShuttle->mPhysicsObject.body->ApplyForce(result * 200.0f, result);    
}

void ShuttleController::OnMouseUp(const Event::MouseUpEvent& event)
{
    mShuttle->mPhysicsObject.body->ResetForces();
}

void ShuttleController::HandleOnShuttlePress()
{
    mono::EventHandler::DispatchEvent(Event::PauseEvent());
    //mShuttle->Fire();
}
