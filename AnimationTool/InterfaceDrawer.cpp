
#include "InterfaceDrawer.h"
#include "UIContext.h"
#include "ImGuiImpl/ImGuiUtils.h"
#include "imgui/imgui.h"

using namespace animator;

InterfaceDrawer::InterfaceDrawer(UIContext& context)
    : m_context(context)
{ }

void InterfaceDrawer::doUpdate(unsigned int delta)
{
    const ImVec2& window_size = ImGui::GetIO().DisplaySize;

    constexpr int padding = 20;
    constexpr int window_width = 280;

    const int animation_window_height = window_size.y - (padding * 2);
    const int window_x = window_size.x - window_width - padding;

    const ImageCoords& add_icon = QuadToImageCoords(m_context.add_icon);
    const ImageCoords& delete_icon = QuadToImageCoords(m_context.delete_icon);
    const ImageCoords& plus_icon = QuadToImageCoords(m_context.plus_icon);
    const ImageCoords& save_icon = QuadToImageCoords(m_context.save_icon);

    const ImVec4 bg_color(1.0f, 1.0f, 1.0f, 1.0f);
    const ImVec4 delete_bg_color(0.0f, 0.0f, 0.0f, 1.0f);
    const ImVec2 small_button_size(22, 22);
    const ImVec2 number_button_size(25, 0);
    const ImVec2 button_size(50, 21);

    const ImVec4& default_color = ImGui::GetStyle().Colors[ImGuiCol_Button];
    const ImVec4& hovered_color = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];

    void* texture_id = reinterpret_cast<void*>(m_context.tools_texture_id);

    ImGui::GetIO().DeltaTime = float(delta) / 1000.0f;
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(window_x, padding));
    ImGui::SetNextWindowSize(ImVec2(window_width, animation_window_height));

    constexpr int animation_flags = ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("Animation", nullptr, ImVec2(0.0f, 0.0f), 1.0f, animation_flags);
    
    if(ImGui::Button("New", button_size))
        m_context.on_add_animation();

    ImGui::SameLine();
    if(ImGui::Button("Delete", button_size))
        m_context.on_delete_animation();

    ImGui::SameLine();
    ImGui::PushID(111);
    if(ImGui::ImageButton(texture_id, small_button_size, save_icon.uv1, save_icon.uv2, 0, delete_bg_color))
        m_context.on_save();
    ImGui::PopID();

    ImGui::Spacing();
    //ImGui::Separator();
    ImGui::Spacing();

    for(int index = 0; index < m_context.animations; ++index)
    {
        char buffer[10] = { 0 };
        snprintf(buffer, 10, "%i", index);

        const ImVec4& color = (index == m_context.animation_id) ? hovered_color : default_color;

        ImGui::PushStyleColor(ImGuiCol_Button, color);
        if(ImGui::Button(buffer, number_button_size))
            m_context.on_set_animation(index);

        ImGui::PopStyleColor();
        ImGui::SameLine();
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    char buffer[100] = { 0 };
    snprintf(buffer, 100, "%s", m_context.display_name);
    if(ImGui::InputText("", buffer, 100))
        m_context.on_name_animation(buffer);

    ImGui::Spacing();
    if(ImGui::Checkbox("Loop", &m_context.loop_animation))
        m_context.on_loop_toggle(m_context.loop_animation);

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::AlignFirstTextHeightToWidgets();
    ImGui::Text("Frames");
    ImGui::SameLine();

    ImGui::PushID(222);
    if(ImGui::ImageButton(texture_id, small_button_size, plus_icon.uv1, plus_icon.uv2, 0, delete_bg_color))
        m_context.on_add_frame();
    ImGui::PopID();

    ImGui::Separator();
    ImGui::BeginChild(666);

    for(size_t index = 0; index < m_context.frames->size(); ++index)
    {
        mono::Frame& frame = m_context.frames->at(index);

        ImGui::Spacing();

        ImGui::PushID(index);
        ImGui::SliderInt("frame", &frame.frame, 0, m_context.max_frame_id);

        ImGui::SameLine(0.0f, 20.0f);
        if(ImGui::ImageButton(texture_id, small_button_size, delete_icon.uv1, delete_icon.uv2, 0, delete_bg_color))
            m_context.on_delete_frame(index);

        ImGui::SliderInt("duration", &frame.duration, 10, 1000, "%.0f ms");
        ImGui::PopID();

        ImGui::Spacing();
        ImGui::Separator();
    }

    ImGui::EndChild();
    ImGui::End();

    //ImGui::ShowTestWindow();
    //ImGui::ShowStyleEditor();
    ImGui::Render();
}
