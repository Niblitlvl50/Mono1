
#include "File.h"
#include <cstdio>

namespace
{
    File::FilePtr OpenFile(const char* file_name, const char* mode)
    {
        FILE* file = std::fopen(file_name, mode);
        if(!file)
        {
            std::printf("System - Unable to open file: %s, with mode: %s\n", file_name, mode);
            return File::FilePtr(nullptr, nullptr);
        }

        return File::FilePtr(file, fclose);
    }
}

File::FilePtr File::OpenBinaryFile(const char* file_name)
{
    return OpenFile(file_name, "rb");
}

File::FilePtr File::OpenAsciiFile(const char* file_name)
{
    return OpenFile(file_name, "r");
}

File::FilePtr File::CreateBinaryFile(const char* file_name)
{
    return OpenFile(file_name, "wb");
}

File::FilePtr File::CreateAsciiFile(const char* file_name)
{
    return OpenFile(file_name, "w");
}

long File::FileSize(const FilePtr& file)
{
    std::fseek(file.get(), 0, SEEK_END);
	const long size = std::ftell(file.get());
    std::rewind(file.get());
    
    return size;
}

void File::FileRead(const FilePtr& file, std::vector<byte>& bytes)
{
    const long filesize = File::FileSize(file);
    
    bytes.resize(filesize, 0);
    std::fread(bytes.data(), 1, filesize, file.get());
}

