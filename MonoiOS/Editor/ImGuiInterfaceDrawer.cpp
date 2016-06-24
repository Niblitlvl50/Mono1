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

void ImGuiInterfaceDrawer::doUpdate(unsigned int delta)
{
    ImGui::GetIO().DeltaTime = float(delta) / 1000.0f;

    static float f = 0.0f;

    ImGui::NewFrame();

    ImGui::Begin("My Window", nullptr, ImVec2(500, 700));
    ImGui::Text("Hello world!");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::End();

    ImGui::ShowTestWindow();

    ImGui::Render();
}
