
#include "InputSystem.h"
#include "System/Hash.h"
#include "EventHandler/EventHandler.h"

#include "Events/EventFuncFwd.h"
#include "Util/Algorithm.h"

using namespace mono;

InputSystem::InputSystem(mono::EventHandler* event_handler)
    : m_event_handler(event_handler)
{
    using namespace std::placeholders;

    const event::MouseMotionEventFunc mouse_motion_func = std::bind(&InputSystem::OnMouseMotionEvent, this, _1);
    const event::MouseDownEventFunc mouse_down_func = std::bind(&InputSystem::OnMouseDownEvent, this, _1);
    const event::MouseUpEventFunc mouse_up_func = std::bind(&InputSystem::OnMouseUpEvent, this, _1);
    const event::MouseWheelEventFunc mouse_wheel_func = std::bind(&InputSystem::OnMouseWheelEvent, this, _1);

    m_mouse_motion_token = m_event_handler->AddListener(mouse_motion_func);
    m_mouse_down_token = m_event_handler->AddListener(mouse_down_func);
    m_mouse_up_token = m_event_handler->AddListener(mouse_up_func);
    m_mouse_wheel_token = m_event_handler->AddListener(mouse_wheel_func);

    const event::KeyDownEventFunc key_down_func = std::bind(&InputSystem::OnKeyDown, this, _1);
    const event::KeyUpEventFunc key_up_func = std::bind(&InputSystem::OnKeyUp, this, _1);
    m_key_down_token = m_event_handler->AddListener(key_down_func);
    m_key_up_token = m_event_handler->AddListener(key_up_func);

    const event::ControllerButtonDownFunc controller_button_donw_func = std::bind(&InputSystem::OnControllerButton, this, _1);
    const event::ControllerAxisFunc controller_axis_func = std::bind(&InputSystem::OnControllerAxis, this, _1);
    m_controller_down_token = m_event_handler->AddListener(controller_button_donw_func);
    m_controller_axis_token = m_event_handler->AddListener(controller_axis_func);
}

InputSystem::~InputSystem()
{
    m_event_handler->RemoveListener(m_mouse_motion_token);
    m_event_handler->RemoveListener(m_mouse_down_token);
    m_event_handler->RemoveListener(m_mouse_up_token);
    m_event_handler->RemoveListener(m_mouse_wheel_token);
    m_event_handler->RemoveListener(m_key_down_token);
    m_event_handler->RemoveListener(m_key_up_token);
    m_event_handler->RemoveListener(m_controller_down_token);
    m_event_handler->RemoveListener(m_controller_axis_token);
}

uint32_t InputSystem::Id() const
{
    return hash::Hash(Name());
}

const char* InputSystem::Name() const
{
    return "inputsystem";
}

void InputSystem::Update(const mono::UpdateContext& update_context)
{

}

mono::InputContext* InputSystem::CreateContext(int priority, InputContextBehaviour context_behaviour)
{
    mono::InputContext* context = new mono::InputContext;
    context->enabled = true;
    context->priority = priority;
    context->behaviour = context_behaviour;
    context->mouse_input = nullptr;
    context->keyboard_input = nullptr;
    context->controller_input = nullptr;

    m_contexts.push_back(context);

    const auto sort_by_priority = [](const mono::InputContext* left, const mono::InputContext* right) {
        return left->priority < right->priority;
    };
    std::sort(m_contexts.begin(), m_contexts.end(), sort_by_priority);

    return context;
}

void InputSystem::ReleaseContext(mono::InputContext* context)
{
    mono::remove(m_contexts, context);
    delete context;
}

mono::EventResult InputSystem::OnMouseMotionEvent(const event::MouseMotionEvent& event)
{
    const auto apply_event = [&event](InputContext* context) {
        if(context->mouse_input)
            return context->mouse_input->Move(event);
        return mono::InputResult::Pass;
    };
    ForEachContext(apply_event);

    return mono::EventResult::PASS_ON;
}

mono::EventResult InputSystem::OnMouseDownEvent(const event::MouseDownEvent& event)
{
    const auto apply_event = [&event](InputContext* context) {
        if(context->mouse_input)
            return context->mouse_input->ButtonDown(event);
        return mono::InputResult::Pass;
    };
    ForEachContext(apply_event);

    return mono::EventResult::PASS_ON;
}

mono::EventResult InputSystem::OnMouseUpEvent(const event::MouseUpEvent& event)
{
    const auto apply_event = [&event](InputContext* context) {
        if(context->mouse_input)
            return context->mouse_input->ButtonUp(event);
        return mono::InputResult::Pass;
    };
    ForEachContext(apply_event);

    return mono::EventResult::PASS_ON;
}

mono::EventResult InputSystem::OnMouseWheelEvent(const event::MouseWheelEvent& event)
{
    const auto apply_event = [&event](InputContext* context) {
        if(context->mouse_input)
            return context->mouse_input->Wheel(event);
        return mono::InputResult::Pass;
    };
    ForEachContext(apply_event);

    return mono::EventResult::PASS_ON;
}

mono::EventResult InputSystem::OnKeyDown(const event::KeyDownEvent& event)
{
    const auto apply_event = [&event](InputContext* context) {
        if(context->keyboard_input)
            return context->keyboard_input->KeyDown(event);
        return mono::InputResult::Pass;
    };
    ForEachContext(apply_event);

    return mono::EventResult::PASS_ON;
}

mono::EventResult InputSystem::OnKeyUp(const event::KeyUpEvent& event)
{
    const auto apply_event = [&event](InputContext* context) {
        if(context->keyboard_input)
            return context->keyboard_input->KeyUp(event);
        return mono::InputResult::Pass;
    };
    ForEachContext(apply_event);

    return mono::EventResult::PASS_ON;
}

mono::EventResult InputSystem::OnControllerButton(const event::ControllerButtonDownEvent& event)
{
    const auto apply_event = [&event](InputContext* context) {
        if(context->controller_input)
            return context->controller_input->ButtonDown(event);
        return mono::InputResult::Pass;
    };
    ForEachContext(apply_event);

    return mono::EventResult::PASS_ON;
}

mono::EventResult InputSystem::OnControllerAxis(const event::ControllerAxisEvent& event)
{
    const auto apply_event = [&event](InputContext* context) {
        if(context->controller_input)
            return context->controller_input->Axis(event);
        return mono::InputResult::Pass;
    };
    ForEachContext(apply_event);

    return mono::EventResult::PASS_ON;
}

