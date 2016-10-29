
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

    ImGui::SameLine(0.0f, 150);
    if(ImGui::Button("Add frame"))
        m_context.on_add_frame();

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::BeginChild(666);

    for(size_t index = 0; index < m_context.frames->size(); ++index)
    {
        mono::Frame& frame = m_context.frames->at(index);

        ImGui::Spacing();

        ImGui::PushID(index);
        ImGui::SliderInt("frame", &frame.frame, 0, m_context.max_frame_id);

        ImGui::SameLine(0.0f, 20.0f);
        if(ImGui::SmallButton("x"))
            m_context.on_delete_frame(index);

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

    ImGui::Begin("tools", nullptr, ImVec2(0.0f, 0.0f), 0.0f, tools_flags);

    void* texture_id = reinterpret_cast<void*>(m_context.tools_texture_id);
    const ImVec2 button_size(44, 44);
    const ImVec4 bg_color(1.0f, 1.0f, 1.0f, 1.0f);

    const ImVec2 save1(m_context.save_icon.mA.x, m_context.save_icon.mB.y);
    const ImVec2 save2(m_context.save_icon.mB.x, m_context.save_icon.mA.y);

    ImGui::Indent(215);

    if(ImGui::ImageButton(texture_id, button_size, save1, save2, 2, bg_color))
        m_context.on_save();

    ImGui::End();

    //ImGui::ShowTestWindow();
    ImGui::Render();
}
