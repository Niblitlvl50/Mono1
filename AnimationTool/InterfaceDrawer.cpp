
#include "InterfaceDrawer.h"
#include "UIContext.h"
#include "imgui/imgui.h"

using namespace animator;

InterfaceDrawer::InterfaceDrawer(UIContext& context)
    : m_context(context)
{ }

void InterfaceDrawer::doUpdate(unsigned int delta)
{
    ImGui::GetIO().DeltaTime = float(delta) / 1000.0f;
    ImGui::NewFrame();

    const ImVec2& window_size = ImGui::GetIO().DisplaySize;

    constexpr int padding = 20;
    constexpr int window_width = 280;
    constexpr int tools_window_height = 64;

    const int animation_window_height = window_size.y - (padding * 2) - 10 - tools_window_height;
    const int window_x = window_size.x - window_width - padding;
    const int tools_window_y = animation_window_height + padding + 10;

    ImGui::SetNextWindowPos(ImVec2(window_x, padding));
    ImGui::SetNextWindowSize(ImVec2(window_width, animation_window_height));

    constexpr int animation_flags = ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("Animation", nullptr, ImVec2(0.0f, 0.0f), 1.0f, animation_flags);
    
    ImGui::Value("Id", m_context.animation_id);
    ImGui::SameLine();
    ImGui::Text("(%s)", m_context.display_name);

    if(ImGui::Checkbox("Loop", &m_context.loop_animation))
        m_context.on_loop_toggle(m_context.loop_animation);

    ImGui::Spacing();
    ImGui::Text("Frames");
    ImGui::Separator();

    ImGui::BeginChild(666);

    for(size_t index = 0; index < m_context.frames->size(); ++index)
    {
        mono::Frame& frame = m_context.frames->at(index);

        ImGui::Spacing();

        ImGui::PushID(index);
        ImGui::InputInt("frame", &frame.frame);
        ImGui::SliderInt("duration", &frame.duration, 10, 1000, "%.0f ms");
        ImGui::PopID();

        ImGui::Spacing();
        ImGui::Separator();
    }

    ImGui::EndChild();
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(window_x, tools_window_y));
    ImGui::SetNextWindowSize(ImVec2(window_width, 64));

    constexpr int tools_flags = ImGuiWindowFlags_NoTitleBar |
                                ImGuiWindowFlags_NoResize |
                                ImGuiWindowFlags_NoMove | 
                                ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("tools", nullptr, ImVec2(0.0f, 0.0f), 1.0f, tools_flags);

    const ImVec2 button_size(44, 44);

    ImGui::PushID(1);
    if(ImGui::ImageButton(reinterpret_cast<void*>(m_context.zoom_in_icon), button_size, ImVec2(0,0), ImVec2(1,1), 2))
        m_context.on_tool_callback(Action::ZOOM_IN);
    ImGui::PopID();

    ImGui::SameLine();
    ImGui::PushID(2);
    if(ImGui::ImageButton(reinterpret_cast<void*>(m_context.zoom_out_icon), button_size, ImVec2(0,0), ImVec2(1,1), 2))
        m_context.on_tool_callback(Action::ZOOM_OUT);
    ImGui::PopID();

    ImGui::SameLine();
    ImGui::PushID(3);
    if(ImGui::ImageButton(reinterpret_cast<void*>(m_context.save_icon), button_size, ImVec2(0,0), ImVec2(1,1), 2))
        m_context.on_tool_callback(Action::SAVE);
    ImGui::PopID();

    ImGui::End();

    ImGui::Render();
}
