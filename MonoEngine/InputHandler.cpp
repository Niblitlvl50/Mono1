/*
 *  InputHandler.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "InputHandler.h"
#include "EventHandler.h"

#include "QuitEvent.h"
#include "SurfaceChangedEvent.h"
#include "ActivatedEvent.h"
#include "KeyDownEvent.h"
#include "KeyUpEvent.h"
#include "MouseDownEvent.h"
#include "MouseUpEvent.h"
#include "MouseMotionEvent.h"
#include "MouseWheelEvent.h"
#include "TouchEvent.h"
#include "MultiGestureEvent.h"


using namespace mono;

InputHandler::InputHandler(const CoordinateFunc& func, EventHandler& eventHandler)
    : mScreenToWorldFunc(func),
      mEventHandler(eventHandler)
{ }

void InputHandler::OnKeyDown(int key)
{
    const Event::KeyDownEvent event(key);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnKeyUp(int key)
{
    const Event::KeyUpEvent event(key);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnMouseDown(unsigned int button, int x, int y)
{
    float worldX = x;
    float worldY = y;
    mScreenToWorldFunc(worldX, worldY);
    const Event::MouseDownEvent event(button, x, y, worldX, worldY);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnMouseUp(unsigned int button, int x, int y)
{
    float worldX = x;
    float worldY = y;
    mScreenToWorldFunc(worldX, worldY);
    const Event::MouseUpEvent event(button, x, y, worldX, worldY);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnMouseMotion(int x, int y)
{
    float worldX = x;
    float worldY = y;
    mScreenToWorldFunc(worldX, worldY);
    const Event::MouseMotionEvent event(x, y, worldX, worldY);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnMouseWheel(int x, int y)
{
    const Event::MouseWheelEvent event(x, y);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnTouchDown(float x, float y, float dx, float dy)
{
    const Event::TouchEvent event(Event::TouchType::DOWN, x, y, dx, dy);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnTouchUp(float x, float y, float dx, float dy)
{
    const Event::TouchEvent event(Event::TouchType::UP, x, y, dx, dy);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnTouchMotion(float x, float y, float dx, float dy)
{
    const Event::TouchEvent event(Event::TouchType::MOTION, x, y, dx, dy);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnMultiGesture(float x, float y, float theta, float distance)
{
    const Event::MultiGestureEvent event(x, y, theta, distance);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnUserEvent(int, void*, void*)
{ }

void InputHandler::OnQuit()
{
    const Event::QuitEvent event;
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnSurfaceChanged(int width, int height)
{
    const Event::SurfaceChangedEvent event(width, height);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnActivated(bool gain)
{
    const Event::ActivatedEvent event(gain);
    mEventHandler.DispatchEvent(event);
}


