//
//  RenderUtils.cpp
//  Mono1
//
//  Created by Niblit on 2013-03-17.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "RenderUtils.h"
#include "Quad.h"
#include "Texture.h"
#include "Sprite.h"
#include "SysOpenGL.h"

#include <cmath>

void mono::DrawQuad(const math::Quad& quad, const mono::Color& color, bool filled)
{
    const float vertices[] = { quad.mA.mX, quad.mA.mY,
                               quad.mB.mX, quad.mA.mY,
                               quad.mB.mX, quad.mB.mY,
                               quad.mA.mX, quad.mB.mY };
    
    mono::Texture::Clear();
    glColor4f(color.red, color.green, color.blue, color.alpha);

    glEnableClientState(GL_VERTEX_ARRAY);
    
    const GLenum primitive = filled ? GL_POLYGON : GL_LINE_LOOP;
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(primitive, 0, 4);
    
    glDisableClientState(GL_VERTEX_ARRAY);        
}

void mono::DrawCircle(const math::Vector2f& position, float radie, int segments, const mono::Color& color)
{
    std::vector<float> vertices;
    const float coef = 2.0f * math::PI / float(segments);
    
	for(int index = 0; index < segments; ++index)
	{
		const float radians = index * coef;
		const float x = radie * std::cos(radians) + position.mX;
		const float y = radie * std::sin(radians) + position.mY;
        
        vertices.push_back(x);
        vertices.push_back(y);
	}
    
    mono::Texture::Clear();
    glColor4f(color.red, color.green, color.blue, color.alpha);
 
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glVertexPointer(2, GL_FLOAT, 0, vertices.data());
    glDrawArrays(GL_LINE_LOOP, 0, segments);
    
    glDisableClientState(GL_VERTEX_ARRAY);
}

void mono::DrawSprite(const mono::Sprite& sprite)
{
    static const float vertices[] = { -0.5f, -0.5f,
                                      -0.5f,  0.5f,
                                       0.5f,  0.5f,
                                       0.5f, -0.5f };
    static const unsigned short indices[] = { 0, 2, 1, 0, 3, 2 };

    sprite.GetTexture()->Use();
    const math::Quad& quad = sprite.GetTextureCoords();
    const float coords[] = { quad.mA.mX, quad.mA.mY,
                             quad.mA.mX, quad.mB.mY,
                             quad.mB.mX, quad.mB.mY,
                             quad.mB.mX, quad.mA.mY };
    
    const mono::Color& color = sprite.GetShade();
    glColor4f(color.red, color.green, color.blue, color.alpha);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, coords);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


