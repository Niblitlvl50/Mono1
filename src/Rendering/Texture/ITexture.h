
#pragma once

namespace mono
{
    class ITexture
    {
    public:

        virtual ~ITexture() = default;

        //! Use the texture
        virtual void Use() const = 0;

        //! Get the internal texture id
        virtual unsigned int Id() const = 0;

        //! Get the width of the texture
        //! \return int
        virtual unsigned int Width() const = 0;

        //! Get the height of the texture
        //! \return int
        virtual unsigned int Height() const = 0;
    };

    void ClearTexture();
}

