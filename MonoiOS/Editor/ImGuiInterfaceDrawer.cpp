//
//  ImGuiInterfaceDrawer.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 20/06/16.
//
//

#include "ImGuiInterfaceDrawer.h"
#include "imgui.h"

using namespace editor;

ImGuiInterfaceDrawer::ImGuiInterfaceDrawer(UIContext& context)
    : m_context(context)
{ }

void ImGuiInterfaceDrawer::doUpdate(unsigned int delta)
{
    ImGui::GetIO().DeltaTime = float(delta) / 1000.0f;

    ImGui::NewFrame();

    ImGui::BeginMainMenuBar();
    if(ImGui::BeginMenu("Editor"))
    {
        if(ImGui::MenuItem("Open"))
            m_context.editorMenuCallback(0);

        if(ImGui::MenuItem("Save"))
            m_context.editorMenuCallback(1);

        if(ImGui::MenuItem("Export"))
            m_context.editorMenuCallback(2);

        ImGui::EndMenu();
    }

    if(ImGui::BeginMenu("Tools"))
    {
        if(ImGui::MenuItem("Polygon"))
            m_context.toolsMenuCallback(0);

        if(ImGui::MenuItem("Translate"))
            m_context.toolsMenuCallback(1);

        if(ImGui::MenuItem("Rotate"))
            m_context.toolsMenuCallback(2);

        if(ImGui::MenuItem("Camera"))
            m_context.toolsMenuCallback(3);
        
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();


    ImGui::Begin("Polygons", nullptr, ImVec2(500, 700));

    int selectedPolygon = -1;

    for(int index = 0; index < m_context.polygonItems.size(); ++index)
    {
        const ImGuiTreeNodeFlags selected_flag = (index == m_context.selectedPolygonIndex) ? ImGuiTreeNodeFlags_Selected : 0;
        const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | selected_flag;

        ImGui::TreeNodeEx((void*)(intptr_t)index, flags, m_context.polygonItems.at(index).c_str(), index);
        if(ImGui::IsItemClicked())
            selectedPolygon = index;

        ImGui::SameLine();

        ImGui::PushID(index);
        if(ImGui::SmallButton("Delete"))
            m_context.polygonDeleted(index);

        ImGui::PopID();
    }

    ImGui::End();

    if(selectedPolygon != -1)
        m_context.polygonSelected(selectedPolygon);



    if(m_context.showContextMenu)
    {
        ImGui::OpenPopup("context");
        m_context.showContextMenu = false;
    }

    if(ImGui::BeginPopup("context"))
    {
        ImGui::Text("Create...");
        ImGui::Separator();

        int menu_index = -1;

        for(int index = 0; index < m_context.contextMenuItems.size(); ++index)
        {
            if(ImGui::Selectable(m_context.contextMenuItems.at(index).c_str()))
                menu_index = index;
        }

        ImGui::EndPopup();

        if(menu_index != -1)
            m_context.contextMenuCallback(menu_index);
    }

    constexpr int notification_window_flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoSavedSettings;

    const ImVec2 window_size = ImVec2(160.0f, 50.0f);
    const float window_position = ImGui::GetIO().DisplaySize.x - window_size.x;

    for(int index = 0; index < m_context.notifications.size(); ++index)
    {
        Notification& note = m_context.notifications[index];

        float window_alpha = 0.6f;
        ImColor tint = ImColor(1.0f, 1.0f, 1.0f, 1.0f);

        if(note.time_left < 500)
        {
            const float alpha_scale = float(note.time_left) / 500.0f;
            window_alpha *= alpha_scale;
            tint.Value.w *= alpha_scale;
        }

        note.time_left -= delta;

        char window_id[16];
        std::sprintf(window_id, "overlay: %u", index);

        ImGui::SetNextWindowPos(ImVec2(window_position - 10, index * 60 + 30));

        ImGui::Begin(window_id, nullptr, window_size, window_alpha, notification_window_flags);
        ImGui::Image(reinterpret_cast<void*>(note.image), ImVec2(32.0f, 32.0f), ImVec2(0.0f, 1.0f), ImVec2(0.15f, 0.85f), tint);
        ImGui::SameLine();
        ImGui::TextColored(tint, "%s", note.text.c_str());
        ImGui::End();
    }

    const auto remove_notification_func = [](const Notification& note) {
        return note.time_left <= 0;
    };

    const auto it = std::remove_if(m_context.notifications.begin(), m_context.notifications.end(), remove_notification_func);
    m_context.notifications.erase(it, m_context.notifications.end());

    //ImGui::ShowTestWindow();

    ImGui::Render();
}
