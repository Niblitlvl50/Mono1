//
//  CameraController.cpp
//  Mono1
//
//  Created by Niblit on 2012-08-26.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "CameraController.h"
#include "EventHandler.h"
#include "MultiGestureEvent.h"
#include "ICamera.h"
#include "MathFunctions.h"
#include "Quad.h"

#include <functional>

using namespace mono;


CameraController::CameraController(ICamera* cam)
    : mCam(cam),
      mEnabled(false)
{
    using namespace std::placeholders;
    
    const Event::MultiGestureEventFunc multiGestureFunc = std::bind(&CameraController::OnMultiGesture, this, _1);
    mMultiGestureToken = mono::EventHandler::AddListener(multiGestureFunc);
}
CameraController::~CameraController()
{
    mono::EventHandler::RemoveListener(mMultiGestureToken);
}

void CameraController::OnMultiGesture(const Event::MultiGestureEvent& event)
{
    if(!mEnabled)
        return;
    
    const float multiplier = (event.mDistance < 0.0f) ? 1.0f : -1.0f;
    const float resizeValue = 50.0f * multiplier;
    
    math::Quad quad = mCam->GetViewport();
    const float width = quad.mB.mX - quad.mA.mX;
    const float height = quad.mB.mY - quad.mA.mY;
    
    const float aspect = width / height;
    math::ResizeQuad(quad, resizeValue, aspect);
    
    mCam->SetTargetViewport(quad);
}

