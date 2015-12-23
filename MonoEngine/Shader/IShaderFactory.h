//
//  IShaderFactory.h
//  MonoOSX
//
//  Created by Niklas Damberg on 26/12/14.
//
//

#pragma once

#include <memory>

namespace mono
{
    class ITextureShader;
    class IColorShader;
    class IMorphingShader;

    class IShaderFactory
    {
    public:

        virtual ~IShaderFactory()
        { }

        virtual std::shared_ptr<ITextureShader> CreateTextureShader() const = 0;
        virtual std::shared_ptr<IColorShader> CreateColorShader() const = 0;
        virtual std::shared_ptr<IMorphingShader> CreateMorphingShader() const = 0;
    };
}
