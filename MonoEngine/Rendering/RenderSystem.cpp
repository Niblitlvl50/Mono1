
#include "RenderSystem.h"

#include "Rendering/Text/TextFunctions.h"
#include "Rendering/Shader/ShaderFunctions.h"

void mono::InitializeRender()
{
    mono::LoadDefaultShaderFactory();
}

void mono::ShutdownRender()
{
    mono::UnloadShaderFactory();
    mono::UnloadFonts();
}
