
#include "File.h"
#include <cstdio>

namespace
{
    file::FilePtr OpenFile(const char* file_name, const char* mode)
    {
        FILE* file = std::fopen(file_name, mode);
        if(!file)
        {
            std::printf("file|Unable to open file: %s, with mode: %s\n", file_name, mode);
            return file::FilePtr(nullptr, nullptr);
        }

        return file::FilePtr(file, fclose);
    }
}

file::FilePtr file::OpenBinaryFile(const char* file_name)
{
    return OpenFile(file_name, "rb");
}

file::FilePtr file::OpenAsciiFile(const char* file_name)
{
    return OpenFile(file_name, "r");
}

file::FilePtr file::CreateBinaryFile(const char* file_name)
{
    return OpenFile(file_name, "wb");
}

file::FilePtr file::CreateAsciiFile(const char* file_name)
{
    return OpenFile(file_name, "w");
}

long file::FileSize(const FilePtr& file)
{
    std::fseek(file.get(), 0, SEEK_END);
	const long size = std::ftell(file.get());
    std::rewind(file.get());
    
    return size;
}

void file::FileRead(const FilePtr& file, std::vector<byte>& bytes)
{
    const long filesize = file::FileSize(file);
    
    bytes.resize(filesize, 0);
    std::fread(bytes.data(), 1, filesize, file.get());
}

#include <sys/stat.h>

bool file::Exists(const char* file_name)
{
    struct stat sb;
    const int result = stat(file_name, &sb);
    return (result == 0 && S_ISREG(sb.st_mode));
}
