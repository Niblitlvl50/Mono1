
#include "ImGuiWidgets.h"
#include "ImGuiImpl.h"
#include <string>

void mono::DrawBitFieldType(
    uint32_t& properties, const uint32_t* possible_properties, uint32_t n_properties, mono::BitFieldTextCallback text_callback)
{
    std::string button_text = std::string("[") + text_callback(0) + "]";

    if(properties != 0)
    {
        button_text.clear();
        for(uint32_t prop_index = 0; prop_index < n_properties; ++prop_index)
        {
            const uint32_t prop = possible_properties[prop_index];
            if(properties & prop)
            {
                button_text += text_callback(prop);
                button_text += "|";
            }
        }

        button_text.pop_back();
    }

    const bool pushed = ImGui::Button(button_text.c_str());
    if(pushed)
        ImGui::OpenPopup("entity_properties_select");

    if(ImGui::BeginPopup("entity_properties_select"))
    {
        for(uint32_t prop_index = 0; prop_index < n_properties; ++prop_index)
        {
            const uint32_t prop = possible_properties[prop_index];
            if(ImGui::Selectable(text_callback(prop), properties & prop, ImGuiSelectableFlags_DontClosePopups))
                properties ^= prop;
        }

        ImGui::EndPopup();
    }
}
