//
//  ImGuiDrawable.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 20/06/16.
//
//

#include "ImGuiRenderer.h"
#include "Quad.h"
#include "SysOpenGL.h"
#include "SysTypes.h"
#include "ImGuiShader.h"
#include "ITexture.h"
#include "TextureFactory.h"
#include "IRenderer.h"

#include "imgui.h"

using namespace editor;

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

ImGuiRenderer::ImGuiRenderer(const math::Vector2f& window_size)
    : m_windowSize(window_size)
{
    Initialize();
}

ImGuiRenderer::ImGuiRenderer(const math::Vector2f& window_size, const std::unordered_map<unsigned int, mono::ITexturePtr>& textures)
    : m_windowSize(window_size),
      m_textures(textures)
{
    Initialize();
}

ImGuiRenderer::~ImGuiRenderer()
{
    ImGui::Shutdown();
}

void ImGuiRenderer::Initialize()
{
    m_shader = std::make_shared<editor::ImGuiShader>();

    m_projection = math::Ortho(0.0f, m_windowSize.x, m_windowSize.y, 0.0f, -10.0f, 10.0f);
    math::Identity(m_modelView);

    int width;
    int height;
    byte* pixels;
    ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    mono::ITexturePtr font_texture = mono::CreateTexture(pixels, width, height, GL_RGBA, GL_RGBA);
    m_textures.insert(std::make_pair(font_texture->Id(), font_texture));

    ImGui::GetIO().Fonts->TexID = (void *)(intptr_t)font_texture->Id();
    ImGui::GetIO().DisplaySize = ImVec2(m_windowSize.x, m_windowSize.y);
}

void ImGuiRenderer::doDraw(mono::IRenderer& renderer) const
{
    m_shader->Use();
    m_shader->LoadProjectionMatrix(m_projection);
    m_shader->LoadModelViewMatrix(m_modelView);

    glEnable(GL_SCISSOR_TEST);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    const ImDrawData* draw_data = ImGui::GetDrawData();

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->VtxBuffer.front();
        const ImDrawIdx* idx_buffer = &cmd_list->IdxBuffer.front();

        const void* vertices = (void*)(vtx_buffer + OFFSETOF(ImDrawVert, pos));
        const void* coords = (void*)(vtx_buffer + OFFSETOF(ImDrawVert, uv));
        const void* colors = (void*)(vtx_buffer + OFFSETOF(ImDrawVert, col));

        glVertexAttribPointer(m_shader->PositionAttribute(), 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), vertices);
        glVertexAttribPointer(m_shader->TextureAttribute(), 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), coords);
        glVertexAttribPointer(m_shader->ColorAttribute(), 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), colors);

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
                const auto it = m_textures.find((unsigned int)(intptr_t)id);
                if(it != m_textures.end())
                    renderer.UseTexture(it->second);
                else
                    renderer.ClearTexture();

                glScissor((int)pcmd->ClipRect.x,
                          (int)(m_windowSize.y - pcmd->ClipRect.w),
                          (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
                          (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                
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

math::Quad ImGuiRenderer::BoundingBox() const
{
    return math::Quad(math::zeroVec, m_windowSize);
}
