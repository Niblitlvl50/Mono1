//
//  ShaderFunctions.h
//  MonoOSX
//
//  Created by Niklas Damberg on 26/12/14.
//
//

#pragma once

#include "SysTypes.h"

namespace mono
{
    class IShaderFactory;

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
    uint CompileShader(ShaderType type, const char* source);

    //! Link a shader program, will throw std::runtime_error
    //! if linking failed
    //!
    //! @param vertexShader A compiled vertex shader
    //! @param fragmentShader A compiled fragment shader
    //!
    //! @return A program id
    uint LinkProgram(uint vertexShader, uint fragmentShader);

    //! Load a default implementation of the shader factory
    void LoadDefaultShaderFactory();

    //! Load a custom impementation of the shader factory, used for unit tests
    //! @param factory Cusom implementation to a shader factory
    void LoadCustomShaderFactory(const mono::IShaderFactory* factory);

    //! Get the shader factory
    //! @return The loaded shader factory
    const mono::IShaderFactory* GetShaderFactory();

    //! Clean up the shader factory
    void UnloadShaderFactory();
}
