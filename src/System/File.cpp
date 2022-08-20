
#include "File.h"
#include "System.h"
#include "Debug.h"

#include <cstdio>
#include <string_view>
#include <filesystem>

namespace
{
    file::FilePtr OpenFile(const char* file_name, const char* mode)
    {
        FILE* file = std::fopen(file_name, mode);
        if(!file)
        {
            System::Log("file|Unable to open file: %s, with mode: %s.", file_name, mode);
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

uint32_t file::FileSize(const char* file_name)
{
    return std::filesystem::file_size(file_name);
}

std::vector<byte> file::FileReadAll(const char* file_name)
{
    const uint32_t file_size = FileSize(file_name);
    std::vector<byte> bytes(file_size, 0);

    file::FilePtr file = OpenAsciiFile(file_name);
    const long n_read = std::fread(bytes.data(), sizeof(byte), file_size, file.get());
    MONO_ASSERT(n_read <= file_size);
    bytes.resize(n_read);

    return bytes;
}

std::vector<byte> file::FileReadAllBinary(const char* file_name)
{
    const uint32_t file_size = FileSize(file_name);
    std::vector<byte> bytes(file_size, 0);

    file::FilePtr file = OpenBinaryFile(file_name);
    const long n_read = std::fread(bytes.data(), sizeof(byte), file_size, file.get());
    MONO_ASSERT(n_read <= file_size);
    bytes.resize(n_read);

    return bytes;
}

std::vector<byte> file::FileRead(const file::FilePtr& file)
{
    const long file_size = file::FileSize(file);
    std::vector<byte> bytes(file_size, 0);

    const long n_read = std::fread(bytes.data(), 1, file_size, file.get());
    MONO_ASSERT(n_read <= file_size);
    bytes.resize(n_read);

    return bytes;
}

bool file::Exists(const char* file_name)
{
    return std::filesystem::exists(file_name);
}

bool file::IsExtension(const char* file_name, const char* extension)
{
    const std::string_view filename_view(file_name);
    const size_t dot_pos = filename_view.find_last_of('.');
    if(dot_pos != std::string_view::npos)
    {
        const std::string_view extension_view = filename_view.substr(dot_pos +1);
        return extension_view == extension;
    }

    return false;
}
