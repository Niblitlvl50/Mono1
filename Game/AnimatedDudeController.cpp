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

#include "SDL_keycode.h"

using namespace game;

AnimatedDudeController::AnimatedDudeController(AnimatedDude& dude)
    : mDude(dude)
{
    using namespace std::tr1::placeholders;
    
    const Event::KeyDownEventFunc keyDownFunc = std::tr1::bind(&AnimatedDudeController::OnKeyDown, this, _1);
    mKeyDownToken = mono::EventHandler::AddListener(keyDownFunc);
    
    const Event::MouseUpEventFunc mouseUpFunc = std::tr1::bind(&AnimatedDudeController::OnMouseUp, this, _1);
    mMouseUpToken = mono::EventHandler::AddListener(mouseUpFunc);    
}

AnimatedDudeController::~AnimatedDudeController()
{
    mono::EventHandler::RemoveListener(mKeyDownToken);
    mono::EventHandler::RemoveListener(mMouseUpToken);
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
    mDude.mTarget = Math::Vector2f(event.mX, event.mY);
}


