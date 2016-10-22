
#include "InterfaceDrawer.h"
#include "imgui/imgui.h"

using namespace animator;

void InterfaceDrawer::doUpdate(unsigned int delta)
{
    ImGui::GetIO().DeltaTime = float(delta) / 1000.0f;
    ImGui::NewFrame();

    ImGui::BeginMainMenuBar();
    if(ImGui::BeginMenu("Animator"))
    {
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();

    ImGui::Begin("Selection", nullptr, ImVec2(200, 120), true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::End();

    ImGui::ShowTestWindow();
    ImGui::Render();
}
