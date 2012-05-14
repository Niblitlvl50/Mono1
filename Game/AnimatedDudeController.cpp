//
//  AnimatedDudeController.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "AnimatedDudeController.h"
#include "AnimatedDude.h"
#include "EventHandler.h"
#include "KeyDownEvent.h"
#include "MouseUpEvent.h"
#include "MouseMotionEvent.h"

#include "SDL_keycode.h"

#include <iostream>

using namespace game;

AnimatedDudeController::AnimatedDudeController(AnimatedDude& dude)
    : mDude(dude)
{
    using namespace std::tr1::placeholders;
    
    const Event::KeyDownEventFunc keyDownFunc = std::tr1::bind(&AnimatedDudeController::OnKeyDown, this, _1);
    mKeyDownToken = mono::EventHandler::AddListener(keyDownFunc);
    
    const Event::MouseUpEventFunc mouseUpFunc = std::tr1::bind(&AnimatedDudeController::OnMouseUp, this, _1);
    mMouseUpToken = mono::EventHandler::AddListener(mouseUpFunc);
    
    const Event::MouseMotionEventFunc mouseMotionFunc = std::tr1::bind(&AnimatedDudeController::OnMouseMotion, this, _1);
    mMouseMotionToken = mono::EventHandler::AddListener(mouseMotionFunc);
}

AnimatedDudeController::~AnimatedDudeController()
{
    mono::EventHandler::RemoveListener(mKeyDownToken);
    mono::EventHandler::RemoveListener(mMouseUpToken);
    mono::EventHandler::RemoveListener(mMouseMotionToken);
}

void AnimatedDudeController::OnKeyDown(const Event::KeyDownEvent& event)
{
    if(event.mKey == SDLK_RIGHT)
        mDude.mTarget.mX += 1.0f;
    else if(event.mKey == SDLK_LEFT)
        mDude.mTarget.mX -= 1.0f;    
    
    if(event.mKey == SDLK_UP)
        mDude.mTarget.mY += 1.0f;
    else if(event.mKey == SDLK_DOWN)
        mDude.mTarget.mY -= 1.0f;
}

void AnimatedDudeController::OnMouseUp(const Event::MouseUpEvent& event)
{
    std::cout << "MouseUp at: x " << event.mX << " - y " << event.mY << std::endl;
    mDude.mTarget = Math::Vector2f(event.mX, event.mY);
}

void AnimatedDudeController::OnMouseMotion(const Event::MouseMotionEvent& event)
{
    //mDude.mTarget = Math::Vector2f(event.mX, event.mY);
}



