
#include "ImGuiWidgets.h"
#include "ImGuiImpl.h"
#include <string>

bool mono::DrawBitFieldType(
    uint32_t& value, const uint32_t* possible_properties, uint32_t n_properties, mono::BitFieldTextCallback text_callback)
{
    std::string button_text = std::string("[") + text_callback(0) + "]";

    if(value != 0)
    {
        button_text.clear();
        for(uint32_t prop_index = 0; prop_index < n_properties; ++prop_index)
        {
            const uint32_t prop = possible_properties[prop_index];
            if(value & prop)
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

    const uint32_t initial_value = value;

    if(ImGui::BeginPopup("entity_properties_select"))
    {
        for(uint32_t prop_index = 0; prop_index < n_properties; ++prop_index)
        {
            const uint32_t prop = possible_properties[prop_index];
            if(ImGui::Selectable(text_callback(prop), value & prop, ImGuiSelectableFlags_DontClosePopups))
                value ^= prop;
        }

        ImGui::EndPopup();
    }

    return initial_value != value;
}
