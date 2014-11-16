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
}

ShuttleController::~ShuttleController()
{
    mEventHandler.RemoveListener(mMouseUpToken);
    mEventHandler.RemoveListener(mMouseDownToken);
    mEventHandler.RemoveListener(mMouseMotionToken);
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

    const math::Vector2f current(event.screenX, event.screenY);
    //const float angle = std::abs(math::AngleBetweenPoints(mMouseDownPosition, current));

    //const bool hzSwipe = (angle < 5 || angle > 175);
    //const bool vtSwipe = (85 < angle && angle < 95);
    
    const math::Vector2f& force = mMouseDownPosition - current;

    /*
    if(hzSwipe)
        mShuttle->ApplyRotationForce(force.mX);
    else if(vtSwipe)
        mShuttle->ApplyThrustForce(force.mY);
    */

    mShuttle->ApplyRotationForce(force.mX);
    mShuttle->ApplyThrustForce(force.mY);
}

void ShuttleController::HandleOnShuttlePress()
{
    mShuttle->Fire();
}
