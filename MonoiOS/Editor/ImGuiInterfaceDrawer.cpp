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

    ImGui::Begin("Polygons", nullptr, ImVec2(500, 700));

    int selectedPolygon = -1;

    for(int index = 0; index < m_context.polygonItems.size(); ++index)
    {
        const ImGuiTreeNodeFlags selected_flag = (index == m_context.selectedPolygonIndex) ? ImGuiTreeNodeFlags_Selected : 0;
        const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | selected_flag;

        ImGui::TreeNodeEx((void*)(intptr_t)index, flags, m_context.polygonItems.at(index).c_str(), index);
        if(ImGui::IsItemClicked())
            selectedPolygon = index;
    }

    ImGui::End();

    if(selectedPolygon != -1)
        m_context.polygonCallback(selectedPolygon);



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

    ImGui::ShowTestWindow();

    ImGui::Render();
}
