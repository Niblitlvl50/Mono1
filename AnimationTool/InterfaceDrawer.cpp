
#include "InterfaceDrawer.h"
#include "UIContext.h"
#include "imgui/imgui.h"

using namespace animator;

namespace
{
    struct ImageCoords
    {
        ImVec2 uv1;
        ImVec2 uv2;
    };

    ImageCoords QuadToImageCoords(const math::Quad& input)
    {
        ImageCoords coords;
        coords.uv1 = ImVec2(input.mA.x, input.mB.y);
        coords.uv2 = ImVec2(input.mB.x, input.mA.y);

        return coords;
    }
}

InterfaceDrawer::InterfaceDrawer(UIContext& context)
    : m_context(context)
{ }

void InterfaceDrawer::doUpdate(unsigned int delta)
{
    const ImVec2& window_size = ImGui::GetIO().DisplaySize;

    constexpr int padding = 20;
    constexpr int window_width = 280;
    constexpr int tools_window_height = 64;

    const int animation_window_height = window_size.y - (padding * 2) - 10 - tools_window_height;
    const int window_x = window_size.x - window_width - padding;
    const int tools_window_y = animation_window_height + padding + 10;

    const ImageCoords& add_icon = QuadToImageCoords(m_context.add_icon);
    const ImageCoords& delete_icon = QuadToImageCoords(m_context.delete_icon);
    const ImageCoords& plus_icon = QuadToImageCoords(m_context.plus_icon);

    const ImVec4 delete_bg_color(0.0f, 0.0f, 0.0f, 1.0f);
    const ImVec2 small_button_size(20, 20);

    void* texture_id = reinterpret_cast<void*>(m_context.tools_texture_id);

    ImGui::GetIO().DeltaTime = float(delta) / 1000.0f;
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(window_x, padding));
    ImGui::SetNextWindowSize(ImVec2(window_width, animation_window_height));

    constexpr int animation_flags = ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("Animation", nullptr, ImVec2(0.0f, 0.0f), 1.0f, animation_flags);
    
    ImGui::AlignFirstTextHeightToWidgets();
    ImGui::Value("Id", m_context.animation_id);
    ImGui::SameLine();

    char buffer[100] = { 0 };
    sprintf(buffer, "%s", m_context.display_name);
    if(ImGui::InputText("", buffer, 100))
        m_context.on_name_animation(buffer);

    if(ImGui::Checkbox("Loop", &m_context.loop_animation))
        m_context.on_loop_toggle(m_context.loop_animation);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::AlignFirstTextHeightToWidgets();
    ImGui::Text("Frames");
    ImGui::SameLine();

    if(ImGui::ImageButton(texture_id, small_button_size, plus_icon.uv1, plus_icon.uv2, 0, delete_bg_color))
        m_context.on_add_frame();

    ImGui::Separator();
    ImGui::BeginChild(666);

    for(size_t index = 0; index < m_context.frames->size(); ++index)
    {
        mono::Frame& frame = m_context.frames->at(index);

        ImGui::Spacing();

        ImGui::PushID(index);
        ImGui::SliderInt("frame", &frame.frame, 1, m_context.max_frame_id);

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

    ImGui::SetNextWindowPos(ImVec2(window_x, tools_window_y));
    ImGui::SetNextWindowSize(ImVec2(window_width, 64));

    constexpr int tools_flags = ImGuiWindowFlags_NoTitleBar |
                                ImGuiWindowFlags_NoResize |
                                ImGuiWindowFlags_NoMove | 
                                ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("tools", nullptr, ImVec2(0.0f, 0.0f), 0.0f, tools_flags);

    const ImVec2 button_size(44, 44);
    const ImVec4 bg_color(1.0f, 1.0f, 1.0f, 1.0f);

    const ImageCoords& save_icon = QuadToImageCoords(m_context.save_icon);

    if(ImGui::Button("Add", button_size))
        m_context.on_add_animation();

    ImGui::SameLine();
    if(ImGui::Button("Delete", button_size))
        m_context.on_delete_animation();

    ImGui::SameLine();
    if(ImGui::ImageButton(texture_id, button_size, save_icon.uv1, save_icon.uv2, 2, bg_color))
        m_context.on_save();

    ImGui::End();
    //ImGui::ShowTestWindow();
    ImGui::Render();
}
