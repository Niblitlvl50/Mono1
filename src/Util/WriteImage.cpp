
#include "WriteImage.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

void mono::WriteImageRGBA(char const* filename, int width, int height, const void* data)
{
    stbi_flip_vertically_on_write(1);

    const int stride_in_bytes = width * height * 4 * sizeof(unsigned char);
    const int result = stbi_write_png(filename, width, height, 4, data, stride_in_bytes);
    if(result == 0)
    {
        // error
    }
}
