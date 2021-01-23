
#include "PipelineImpl.h"
#include "System/System.h"

using namespace mono;

PipelineImpl::PipelineImpl(sg_pipeline pipeline_handle, sg_shader shader_handle)
    : m_pipeline_handle(pipeline_handle)
    , m_shader_handle(shader_handle)
{ }

PipelineImpl::~PipelineImpl()
{
    sg_destroy_shader(m_shader_handle);
    sg_destroy_pipeline(m_pipeline_handle);
}

uint32_t PipelineImpl::Id() const
{
    return m_pipeline_handle.id;
}

void PipelineImpl::Apply()
{
    sg_apply_pipeline(m_pipeline_handle);
}
