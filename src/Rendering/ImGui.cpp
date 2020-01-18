
#include "ImGui.h"

#include "Math/Quad.h"
#include "Math/Matrix.h"

#include "Rendering/Texture/ITexture.h"
#include "Rendering/Texture/TextureFactory.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Shader/IShader.h"
#include "Rendering/Shader/ImGuiShader.h"

#include "System/open_gl.h"
#include "imgui/imgui.h"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

namespace
{
    std::unordered_map<uint32_t, mono::ITexturePtr> g_imgui_textures;
}

void mono::InitializeImGui(mono::ImGuiContext& imgui_context)
{
    ImGui::CreateContext();
    ImGui::GetIO().DisplaySize = ImVec2(imgui_context.window_size.x, imgui_context.window_size.y);

    int width;
    int height;
    unsigned char* pixels;
    ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    mono::ITexturePtr font_texture = mono::CreateTexture(pixels, width, height, 4);
    g_imgui_textures.insert(std::make_pair(font_texture->Id(), font_texture));

    ImGui::GetIO().Fonts->TexID = (void *)(intptr_t)font_texture->Id();
}

void mono::DestroyImGui()
{
    g_imgui_textures.clear();
    ImGui::DestroyContext();
}

void mono::DrawImGui(mono::ImGuiContext& imgui_context, mono::IRenderer& renderer)
{
    const ImDrawData* draw_data = ImGui::GetDrawData();
    if(!draw_data)
        return;

    ImGui::GetIO().DisplaySize = ImVec2(imgui_context.window_size.x, imgui_context.window_size.y);

    const math::Matrix& projection =
        math::Ortho(0.0f, imgui_context.window_size.x, imgui_context.window_size.y, 0.0f, -10.0f, 10.0f);
    constexpr math::Matrix model_view;

    renderer.UseShader(imgui_context.shader.get());
    imgui_context.shader->SetProjectionAndModelView(projection, model_view);

    glEnable(GL_SCISSOR_TEST);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->VtxBuffer.front();
        const ImDrawIdx* idx_buffer = &cmd_list->IdxBuffer.front();

        const void* vertices = (void*)(vtx_buffer + OFFSETOF(ImDrawVert, pos));
        const void* coords = (void*)(vtx_buffer + OFFSETOF(ImDrawVert, uv));
        const void* colors = (void*)(vtx_buffer + OFFSETOF(ImDrawVert, col));

        glVertexAttribPointer(ImGuiShader::GetPositionAttribute(imgui_context.shader.get()), 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), vertices);
        glVertexAttribPointer(ImGuiShader::GetTextureAttribute(imgui_context.shader.get()), 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), coords);
        glVertexAttribPointer(ImGuiShader::GetColorAttribute(imgui_context.shader.get()), 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), colors);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                const void* id = pcmd->TextureId;
                const auto it = g_imgui_textures.find((uint32_t)(intptr_t)id);
                if(it != g_imgui_textures.end())
                    renderer.UseTexture(it->second);
                else
                    renderer.ClearTexture();

                glScissor(
                    (int)pcmd->ClipRect.x, (int)(imgui_context.window_size.y - pcmd->ClipRect.w),
                    (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                
                constexpr GLenum data_type = sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, data_type, idx_buffer);
            }
            
            idx_buffer += pcmd->ElemCount;
        }
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glDisable(GL_SCISSOR_TEST);
}

void mono::SetImGuiConfig(const char* config_file)
{
    ImGui::GetIO().IniFilename = config_file;
}

uint32_t mono::LoadImGuiTexture(const char* texture_file)
{
    mono::ITexturePtr texture = mono::CreateTexture("res/textures/placeholder.png");
    g_imgui_textures.insert(std::make_pair(texture->Id(), texture));
    return texture->Id();
}
