//
//  IShader.h
//  MonoOSX
//
//  Created by Niklas Damberg on 21/12/14.
//
//

#pragma once

namespace mono
{
    class IShader
    {
    public:

        virtual ~IShader()
        { }

        virtual void Use() = 0;
        virtual void Clear() = 0;

        virtual int32_t GetAttribLocation(const char* name) const = 0;
        virtual int32_t GetUniformLocation(const char* name) const = 0;
    };
}