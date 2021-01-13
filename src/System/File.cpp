
#include "File.h"
#include "System.h"
#include <cstdio>

namespace
{
    file::FilePtr OpenFile(const char* file_name, const char* mode)
    {
        FILE* file = std::fopen(file_name, mode);
        if(!file)
        {
            System::Log("file|Unable to open file: %s, with mode: %s\n", file_name, mode);
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

std::vector<byte> file::FileRead(const file::FilePtr& file)
{
    const long file_size = file::FileSize(file);

    std::vector<byte> bytes;
    bytes.resize(file_size, 0);
    std::fread(bytes.data(), 1, file_size, file.get());
    return bytes;
}

#include <sys/stat.h>

bool file::Exists(const char* file_name)
{
#ifdef _WIN32
    return false;
#elif
    struct stat sb;
    const int result = stat(file_name, &sb);
    return (result == 0 && S_ISREG(sb.st_mode));
#endif
}
