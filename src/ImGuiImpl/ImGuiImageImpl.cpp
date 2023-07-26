
#include "ImGuiImageImpl.h"
#include "Rendering/Texture/ITexture.h"

#define SOKOL_IMGUI_NO_SOKOL_APP

#include "imgui/imgui.h"
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_imgui.h"

using namespace mono;

namespace
{
    class ImGuiImageImpl : public IImGuiImage
    {
    public:

        ImGuiImageImpl(mono::ITexturePtr texture)
            : m_texture(texture)
        {
            const simgui_image_desc_t image_desc = {
                m_texture->Id(),
                m_texture->SamplerId()
            };
            m_handle = simgui_make_image(&image_desc);
        }

        ~ImGuiImageImpl()
        {
            simgui_destroy_image(m_handle);
        }
        
        void* TextureHandle() const override
        {
            return simgui_imtextureid(m_handle);
        }

        mono::ITexturePtr m_texture;
        simgui_image_t m_handle;
    };
}

mono::IImGuiImagePtr mono::CreateImGuiImage(mono::ITexturePtr texture)
{
    return std::make_shared<ImGuiImageImpl>(texture);
}
