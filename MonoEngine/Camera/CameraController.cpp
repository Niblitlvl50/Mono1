//
//  CameraController.cpp
//  Mono1
//
//  Created by Niblit on 2012-08-26.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "CameraController.h"
#include "EventHandler.h"
#include "MouseWheelEvent.h"
#include "SysKeycodes.h"
#include "ICamera.h"
#include "MathFunctions.h"
#include "Quad.h"

using namespace mono;


CameraController::CameraController(ICamera* cam)
    : mCam(cam)
{
    using namespace std::tr1::placeholders;
    
    const Event::MouseWheelEventFunc mouseWheelFunc = std::tr1::bind(&CameraController::OnMouseWheel, this, _1);
    mMouseWheelToken = mono::EventHandler::AddListener(mouseWheelFunc);
}
CameraController::~CameraController()
{
    mono::EventHandler::RemoveListener(mMouseWheelToken);
}

void CameraController::OnMouseWheel(const Event::MouseWheelEvent& event)
{
    const float multiplier = (event.mY < 0) ? -1.0f : 1.0f;
    const float resizeValue = 50.0f * multiplier;
    
    Math::Quad quad = mCam->GetViewport();
    const float width = quad.mB.mX - quad.mA.mX;
    const float height = quad.mB.mY - quad.mA.mY;
    
    const float aspect = width / height;
    Math::ResizeQuad(quad, resizeValue, aspect);
    
    mCam->SetTargetViewport(quad);
}

