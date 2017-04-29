
#include "ImGuiRenderer.h"
#include "Math/Quad.h"
#include "Math/Matrix.h"
#include "System/SysOpenGL.h"
#include "ImGuiShader.h"
#include "Rendering/Texture/ITexture.h"
#include "Rendering/Texture/TextureFactory.h"
#include "Rendering/IRenderer.h"

#include "imgui/imgui.h"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

ImGuiRenderer::ImGuiRenderer(const char* config_file,
                             const math::Vector& window_size)
    : m_windowSize(window_size)
{
    Initialize(config_file);
}

ImGuiRenderer::ImGuiRenderer(const char* config_file,
                             const math::Vector& window_size,
                             const std::unordered_map<unsigned int, mono::ITexturePtr>& textures)
    : m_windowSize(window_size),
      m_textures(textures)
{
    Initialize(config_file);
}

ImGuiRenderer::~ImGuiRenderer()
{
    ImGui::Shutdown();
}

void ImGuiRenderer::Initialize(const char* config_file)
{
    ImGui::GetIO().DisplaySize = ImVec2(m_windowSize.x, m_windowSize.y);
    ImGui::GetIO().IniFilename = config_file;

    m_shader = std::make_shared<ImGuiShader>();

    int width;
    int height;
    unsigned char* pixels;
    ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    mono::ITexturePtr font_texture = mono::CreateTexture(pixels, width, height, 4);
    m_textures.insert(std::make_pair(font_texture->Id(), font_texture));

    ImGui::GetIO().Fonts->TexID = (void *)(intptr_t)font_texture->Id();
}

void ImGuiRenderer::doDraw(mono::IRenderer& renderer) const
{
    ImGui::GetIO().DisplaySize = ImVec2(m_windowSize.x, m_windowSize.y);
    
    const math::Matrix& projection = math::Ortho(0.0f, m_windowSize.x, m_windowSize.y, 0.0f, -10.0f, 10.0f);
    constexpr math::Matrix model_view;

    renderer.UseShader(m_shader);
    m_shader->LoadProjectionMatrix(projection);
    m_shader->LoadModelViewMatrix(model_view);

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
    return math::Quad(-FLT_MAX, -FLT_MAX, FLT_MAX, FLT_MAX);
}

void ImGuiRenderer::SetWindowSize(const math::Vector& window_size)
{
    m_windowSize = window_size;
}

