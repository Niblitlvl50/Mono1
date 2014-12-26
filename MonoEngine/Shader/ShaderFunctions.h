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
    //! Compile a shader, will throw std::runtime_error
    //! if compilataion failed
    //!
    //! type Shader type, GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
    //! source The shader source
    //!
    //! return A shader id
    uint CompileShader(uint type, const char* source);

    //! Link a shader program, will throw std::runtime_error
    //! if linking failed
    //!
    //! vertexShader A compiled vertex shader
    //! fragmentShader A compiled fragment shader
    //!
    //! return A program id
    uint LinkProgram(uint vertexShader, uint fragmentShader);
}
