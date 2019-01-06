
#pragma once

#include <stdlib.h>

namespace mono
{
    enum class ShaderType
    {
        VERTEX,
        FRAGMENT
    };

    //! Compile a shader, will throw std::runtime_error
    //! if compilataion failed
    //!
    //! @param type Shader type, VERTEX or FRAGMENT
    //! @param source The shader source
    //!
    //! @return A shader id
    size_t CompileShader(ShaderType type, const char* source);

    //! Link a shader program, will throw std::runtime_error
    //! if linking failed
    //!
    //! @param vertexShader A compiled vertex shader
    //! @param fragmentShader A compiled fragment shader
    //!
    //! @return A program id
    size_t LinkProgram(size_t vertexShader, size_t fragmentShader);

    //! Get the shader factory
    //! @return The loaded shader factory
    const class IShaderFactory* GetShaderFactory();
}
