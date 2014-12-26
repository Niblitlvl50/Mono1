//
//  ShaderFunctions.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 26/12/14.
//
//

#include "ShaderFunctions.h"
#include "SysOpenGL.h"

#include <vector>
#include <string>

uint mono::CompileShader(uint type, const char* source)
{
    const GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);

    GLint compiledSuccessfully = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledSuccessfully);
    if(compiledSuccessfully != GL_TRUE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        GLint outLength = 0;
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &outLength, infoLog.data());

        // We don't need the shader anymore.
        glDeleteShader(shader);

        // Store the compile log
        const std::string log(infoLog.begin(), infoLog.end());

        // Throw here since we could not compile the shader
        throw std::runtime_error("Unable to compile shader: " + std::string(source) + "\nLog: " + log);
    }

    return shader;
}

uint mono::LinkProgram(uint vertexShader, uint fragmentShader)
{
    const GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint linkedSuccessfully = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkedSuccessfully);
    if(linkedSuccessfully != GL_TRUE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        GLint outLength = 0;
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &outLength, infoLog.data());

        // We don't need the program anymore.
        glDeleteProgram(program);

        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Store the link log
        const std::string log(infoLog.begin(), infoLog.end());

        // Throw here since we could not compile the shader
        throw std::runtime_error("Unable to link program. - " + log);
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    return program;
}
