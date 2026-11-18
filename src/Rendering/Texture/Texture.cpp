
#include "Texture.h"
#include "System/System.h"
#include <cstdio>

using namespace mono;

TextureImpl::TextureImpl(
    uint32_t width, uint32_t height, uint32_t color_components, const unsigned char* image_data)
    : m_width(width)
    , m_height(height)
{
    m_pixel_format = SG_PIXELFORMAT_R8;
    if(color_components == 2)
        m_pixel_format = SG_PIXELFORMAT_RG8;
    //else if(color_components == 3)
    //    m_data_format = SG_PIXELFORMAT_RGB8;
    else if(color_components == 4)
        m_pixel_format = SG_PIXELFORMAT_RGBA8;

    sg_image_desc image_desc = {};
    image_desc.width = width;
    image_desc.height = height;
    //image_desc.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
    //image_desc.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
    image_desc.pixel_format = m_pixel_format;
    image_desc.data.subimage[0][0].ptr = image_data;
    image_desc.data.subimage[0][0].size = width * height * color_components * sizeof(unsigned char);

    m_handle = sg_make_image(&image_desc);

    const sg_resource_state state = sg_query_image_state(m_handle);
    if(state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create texture.");

    sg_sampler_desc sampler_desc = { };
    m_sampler_handle = sg_make_sampler(sampler_desc);
}

TextureImpl::TextureImpl(uint32_t width, uint32_t height, uint32_t color_components)
    : m_width(width)
    , m_height(height)
    , m_color_components(color_components)
    , m_dynamic(true)
{
    m_pixel_format = SG_PIXELFORMAT_R8;
    if(color_components == 2)
        m_pixel_format = SG_PIXELFORMAT_RG8;
    else if(color_components == 4)
        m_pixel_format = SG_PIXELFORMAT_RGBA8;

    sg_image_desc image_desc = {};
    image_desc.width = width;
    image_desc.height = height;
    image_desc.usage = SG_USAGE_DYNAMIC;
    image_desc.pixel_format = m_pixel_format;

    m_handle = sg_make_image(&image_desc);

    const sg_resource_state state = sg_query_image_state(m_handle);
    if(state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create dynamic texture.");

    sg_sampler_desc sampler_desc = { };
    sampler_desc.min_filter = SG_FILTER_LINEAR;
    sampler_desc.mag_filter = SG_FILTER_LINEAR;
    sampler_desc.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
    sampler_desc.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
    m_sampler_handle = sg_make_sampler(sampler_desc);
}

TextureImpl::TextureImpl(sg_image image_handle)
    : m_handle(image_handle)
{
    const sg_resource_state state = sg_query_image_state(m_handle);
    if(state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create texture.");

    const sg_image_desc info = sg_query_image_desc(m_handle);
    m_width = info.width;
    m_height = info.height;

    sg_sampler_desc sampler_desc = { };
    m_sampler_handle = sg_make_sampler(sampler_desc);
}

TextureImpl::~TextureImpl()
{
    sg_destroy_sampler(m_sampler_handle);
    sg_destroy_image(m_handle);
}

uint32_t TextureImpl::Id() const
{
    return m_handle.id;
}

uint32_t TextureImpl::SamplerId() const
{
    return m_sampler_handle.id;
}

uint32_t TextureImpl::Width() const
{
    return m_width;
}

uint32_t TextureImpl::Height() const
{
    return m_height;
}

bool TextureImpl::IsAlphaTexture() const
{
    return (m_pixel_format == SG_PIXELFORMAT_R8);
}

void TextureImpl::UpdateData(const unsigned char* image_data)
{
    if(!m_dynamic)
    {
        System::Log("Texture|Unable to update the data of a texture that isn't dynamic.");
        return;
    }

    sg_image_data data = {};
    data.subimage[0][0].ptr = image_data;
    data.subimage[0][0].size = m_width * m_height * m_color_components * sizeof(unsigned char);
    sg_update_image(m_handle, &data);
}
