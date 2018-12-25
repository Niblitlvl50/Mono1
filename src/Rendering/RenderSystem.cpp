
#include "RenderSystem.h"

#include "Rendering/Text/TextFunctions.h"
#include "Rendering/Shader/ShaderFunctions.h"

float s_pixels_per_meter = 32.0f;

void mono::InitializeRender(float pixels_per_meter)
{
    s_pixels_per_meter = pixels_per_meter;
    mono::LoadDefaultShaderFactory();
}

void mono::ShutdownRender()
{
    mono::UnloadShaderFactory();
    mono::UnloadFonts();
}
