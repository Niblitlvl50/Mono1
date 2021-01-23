
#pragma once

#include "Rendering/Pipeline/IPipeline.h"
#include "sokol/sokol_gfx.h"
#include <memory>

namespace mono
{
    class PipelineImpl : public mono::IPipeline
    {
    public:

        PipelineImpl(sg_pipeline pipeline_handle, sg_shader shader_handle);
        ~PipelineImpl();

        uint32_t Id() const override;
        void Apply() override;

        sg_pipeline m_pipeline_handle;
        sg_shader m_shader_handle;
    };
}
