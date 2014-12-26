//
//  ShaderFactory.h
//  MonoOSX
//
//  Created by Niklas Damberg on 21/12/14.
//
//

#pragma once

#include <memory>

namespace mono
{
    class IShader;
    std::shared_ptr<IShader> CreateShader(const char* vertexSource, const char* fragmentSource);
}