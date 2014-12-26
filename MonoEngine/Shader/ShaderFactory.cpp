//
//  ShaderFactory.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 21/12/14.
//
//

#include "ShaderFactory.h"
#include "IShader.h"
#include "SysOpenGL.h"

#include <vector>
#include <string>

namespace
{
    class Shader : public mono::IShader
    {
    public:

        Shader(GLuint program)
            : mProgram(program)
        { }

        ~Shader()
        {
            glDeleteProgram(mProgram);
        }

        virtual void Use()
        {
            if(mProgram != sBoundProgram)
            {
                glUseProgram(mProgram);
                sBoundProgram = mProgram;
            }
        }

        virtual void Clear()
        {
            glUseProgram(0);
            sBoundProgram = -1;
        }

        virtual int32_t GetAttribLocation(const char* name) const
        {
            return glGetAttribLocation(mProgram, name);
        }

        virtual int32_t GetUniformLocation(const char* name) const
        {
            return glGetUniformLocation(mProgram, name);
        }

        GLuint mProgram;
        static GLuint sBoundProgram;
    };

    GLuint Shader::sBoundProgram = -1;


    GLuint CompileShader(GLenum type, const char* source)
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

    GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
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
}

std::shared_ptr<mono::IShader> mono::CreateShader(const char* vertexSource, const char* fragmentSource)
{
    const GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    const GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
    const GLuint program = LinkProgram(vertexShader, fragmentShader);

    return std::make_shared<Shader>(program);
}
