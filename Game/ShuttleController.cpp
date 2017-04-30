
#include "ShuttleController.h"
#include "Shuttle.h"
#include "EventHandler/EventHandler.h"
#include "Events/GameEventFuncFwd.h"
#include "Events/EventFuncFwd.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Events/TimeScaleEvent.h"
#include "System/Keycodes.h"
#include "Weapons/WeaponTypes.h"
#include "Math/MathFunctions.h"


using namespace game;

ShuttleController::ShuttleController(game::Shuttle* shuttle, mono::EventHandler& eventHandler)
    : mShuttle(shuttle),
      mEventHandler(eventHandler),
      mMouseDown(false)
{
    using namespace std::placeholders;

    const event::MouseUpEventFunc mouseUpFunc = std::bind(&ShuttleController::OnMouseUp, this, _1);
    const event::MouseDownEventFunc mouseDownFunc = std::bind(&ShuttleController::OnMouseDown, this, _1);
    const event::MouseMotionEventFunc mouseMotionFunc = std::bind(&ShuttleController::OnMouseMotion, this, _1);
    const event::KeyDownEventFunc keyDownFunc = std::bind(&ShuttleController::OnKeyDown, this, _1);
    const event::KeyUpEventFunc key_up_func = std::bind(&ShuttleController::OnKeyUp, this, _1);

    mMouseUpToken = mEventHandler.AddListener(mouseUpFunc);
    mMouseDownToken = mEventHandler.AddListener(mouseDownFunc);
    mMouseMotionToken = mEventHandler.AddListener(mouseMotionFunc);
    mKeyDownToken = mEventHandler.AddListener(keyDownFunc);
    m_keyUpToken = mEventHandler.AddListener(key_up_func);
}

ShuttleController::~ShuttleController()
{
    mEventHandler.RemoveListener(mMouseUpToken);
    mEventHandler.RemoveListener(mMouseDownToken);
    mEventHandler.RemoveListener(mMouseMotionToken);
    mEventHandler.RemoveListener(mKeyDownToken);
    mEventHandler.RemoveListener(m_keyUpToken);
}

bool ShuttleController::OnMouseDown(const event::MouseDownEvent& event)
{
    mShuttle->Fire();

    mMouseDownPosition = math::Vector(event.screenX, -event.screenY);
    mMouseDown = true;

    return true;
}

bool ShuttleController::OnMouseUp(const event::MouseUpEvent& event)
{
    mShuttle->StopFire();
    
    mShuttle->mPhysicsObject.body->ResetForces();
    mShuttle->SetBoosterThrusting(BoosterPosition::ALL, false);

    mMouseDown = false;

    return true;
}

bool ShuttleController::OnMouseMotion(const event::MouseMotionEvent& event)
{
    if(!mMouseDown)
        return false;

    // Reset forces first.
    mShuttle->mPhysicsObject.body->ResetForces();

    const math::Vector current(event.screenX, -event.screenY);

    math::Vector force = current - mMouseDownPosition;
    math::Normalize(force);

    mShuttle->ApplyImpulse(force * 10);


    const bool leftBoosterOn = (force.x > 0.0f);
    const bool rightBoosterOn = (force.x < 0.0f);
    const bool mainBoosterOn = (force.y > 0.0f);

    mShuttle->SetBoosterThrusting(BoosterPosition::LEFT, leftBoosterOn);
    mShuttle->SetBoosterThrusting(BoosterPosition::RIGHT, rightBoosterOn);
    mShuttle->SetBoosterThrusting(BoosterPosition::MAIN, mainBoosterOn);

    mMouseDownPosition = current;

    return false;
}

bool ShuttleController::OnKeyDown(const event::KeyDownEvent& event)
{
    if(event.key != Key::ONE &&
       event.key != Key::TWO &&
       event.key != Key::THREE &&
       event.key != Key::FOUR && 
       event.key != Key::SPACE)
        return false;

    if(event.key == Key::ONE)
       mShuttle->SelectWeapon(WeaponType::STANDARD);
    else if(event.key == Key::TWO)
        mShuttle->SelectWeapon(WeaponType::ROCKET);
    else if(event.key == Key::THREE)
        mShuttle->SelectWeapon(WeaponType::CACOPLASMA);
    else if(event.key == Key::FOUR)
        mShuttle->SelectWeapon(WeaponType::GENERIC);
    else if(event.key == Key::SPACE)
        mEventHandler.DispatchEvent(event::TimeScaleEvent(0.5f));

    return true;
}

bool ShuttleController::OnKeyUp(const event::KeyUpEvent& event)
{
    if(event.key != Key::SPACE)
        return false;

    mEventHandler.DispatchEvent(event::TimeScaleEvent(1.0f));
    return true;
}
