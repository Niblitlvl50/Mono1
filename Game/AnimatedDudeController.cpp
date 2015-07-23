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
#include "EventFuncFwd.h"
#include "KeyDownEvent.h"
#include "SysKeycodes.h"

using namespace game;

AnimatedDudeController::AnimatedDudeController(AnimatedDude* dude, mono::EventHandler& eventHandler)
    : mDude(dude),
      mEventHandler(eventHandler)
{
    using namespace std::placeholders;
    
    const Event::KeyDownEventFunc keyDownFunc = std::bind(&AnimatedDudeController::OnKeyDown, this, _1);
    mKeyDownToken = mEventHandler.AddListener(keyDownFunc);
}

AnimatedDudeController::~AnimatedDudeController()
{
    mEventHandler.RemoveListener(mKeyDownToken);
}

void AnimatedDudeController::OnKeyDown(const Event::KeyDownEvent& event)
{
    if(event.mKey == Key::RIGHT)
        mDude->mTarget.x += 20.0f;
    else if(event.mKey == Key::LEFT)
        mDude->mTarget.x -= 20.0f;
    
    if(event.mKey == Key::UP)
        mDude->mTarget.y += 20.0f;
    else if(event.mKey == Key::DOWN)
        mDude->mTarget.y -= 20.0f;
}


