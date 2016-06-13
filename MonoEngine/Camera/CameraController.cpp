//
//  CameraController.cpp
//  Mono1
//
//  Created by Niblit on 2012-08-26.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "CameraController.h"
#include "EventHandler.h"
#include "EventFuncFwd.h"
#include "MultiGestureEvent.h"
#include "ICamera.h"
#include "MathFunctions.h"
#include "Quad.h"

#include <functional>
#include <cmath>

using namespace mono;


CameraController::CameraController(ICamera* cam, EventHandler& eventHandler)
    : mCam(cam),
      mEventHandler(eventHandler),
      mEnabled(true)
{
    using namespace std::placeholders;
    
    const event::MultiGestureEventFunc multiGestureFunc = std::bind(&CameraController::OnMultiGesture, this, _1);
    mMultiGestureToken = mEventHandler.AddListener(multiGestureFunc);
}

CameraController::~CameraController()
{
    mEventHandler.RemoveListener(mMultiGestureToken);
}

void CameraController::OnMultiGesture(const event::MultiGestureEvent& event)
{
    if(!mEnabled)
        return;

    if(std::fabs(event.distance) < 1e-3)
        return;

    math::Quad quad = mCam->GetViewport();

    const float multiplier = (event.distance < 0.0f) ? 1.0f : -1.0f;
    const float resizeValue = quad.mB.x * 0.15f * multiplier;
    const float aspect = quad.mB.x / quad.mB.y;
    math::ResizeQuad(quad, resizeValue, aspect);

    mCam->SetTargetViewport(quad);
}

