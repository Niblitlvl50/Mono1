//
//  File.cpp
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "SysFile.h"
#include <cstdio>

namespace
{
    File::FilePtr OpenFile(const char* file_name, const char* mode)
    {
        std::FILE* file = std::fopen(file_name, mode);
        if(!file)
        {
            std::printf("Unable to open file: %s, with mode: %s", file_name, mode);
            return nullptr;
        }

        return File::FilePtr(file, std::fclose);
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

