
#include "TextureShader.h"
#include "ShaderFunctions.h"
#include "Math/Matrix.h"
#include "Rendering/Color.h"

#include "System/open_gl.h"


namespace
{
    constexpr const char* vertexSource = R"(
    #ifdef __IPHONEOS__
        precision mediump float;
    #endif

        attribute vec2 vertex_position;
        attribute vec2 texture_coord;

        uniform mat4 mv_matrix;
        uniform mat4 p_matrix;

        varying vec2 v_texture_coord;

        void main()
        {
            gl_Position = p_matrix * mv_matrix * vec4(vertex_position, 0.0, 1.0);
            v_texture_coord = texture_coord;
        }
    )";

    constexpr const char* fragmentSource = R"(
    #ifdef __IPHONEOS__
        precision mediump float;
    #endif

        varying vec2 v_texture_coord;

        uniform vec4 color_shade;
        uniform sampler2D sampler;
        uniform bool is_alpha_texture;
        uniform float texture_offset;

        void main()
        {
            if(is_alpha_texture)
                gl_FragColor = texture2D(sampler, v_texture_coord).aaaa * color_shade;
            else
                gl_FragColor = texture2D(sampler, v_texture_coord) * color_shade;
        }
    )";
}


using namespace mono;

TextureShader::TextureShader()
{
    const GLuint vertexShader = CompileShader(mono::ShaderType::VERTEX, vertexSource);
    const GLuint fragmentShader = CompileShader(mono::ShaderType::FRAGMENT, fragmentSource);
    
    m_program = LinkProgram(vertexShader, fragmentShader);

    m_MV_matrix_location = glGetUniformLocation(m_program, "mv_matrix");
    m_P_matrix_location = glGetUniformLocation(m_program, "p_matrix");

    m_color_shade_location = glGetUniformLocation(m_program, "color_shade");
    m_is_alpha_texture_location = glGetUniformLocation(m_program, "is_alpha_texture");
    m_texture_offset_location = glGetUniformLocation(m_program, "texture_offset");

    m_position_attribute_location = (unsigned int)glGetAttribLocation(m_program, "vertex_position");
    m_texture_attribute_location = (unsigned int)glGetAttribLocation(m_program, "texture_coord");
}

TextureShader::~TextureShader()
{
    glDeleteProgram(m_program);
}

void TextureShader::Use()
{
    glUseProgram(m_program);
}

unsigned int TextureShader::GetShaderId() const
{
    return m_program;
}

void TextureShader::LoadProjectionMatrix(const math::Matrix& projection)
{
    glUniformMatrix4fv(m_P_matrix_location, 1, GL_FALSE, projection.data);
}

void TextureShader::LoadModelViewMatrix(const math::Matrix& modelView)
{
    glUniformMatrix4fv(m_MV_matrix_location, 1, GL_FALSE, modelView.data);
}

unsigned int TextureShader::GetPositionAttributeLocation() const
{
    return m_position_attribute_location;
}

unsigned int TextureShader::GetTextureAttributeLocation() const
{
    return m_texture_attribute_location;
}

void TextureShader::SetShade(const mono::Color::RGBA& shade)
{
    glUniform4f(m_color_shade_location, shade.red, shade.green, shade.blue, shade.alpha);
}

void TextureShader::SetAlphaTexture(bool isAlpha)
{
    glUniform1i(m_is_alpha_texture_location, isAlpha);
}

void TextureShader::SetTextureOffset(float offset)
{
    glUniform1f(m_texture_offset_location, offset);
}
