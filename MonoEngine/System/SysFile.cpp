//
//  File.cpp
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "SysFile.h"
#include <stdexcept>
#include <string>
#include <cstdio>

File::FilePtr File::OpenBinaryFile(const char* source)
{
    std::FILE* file = std::fopen(source, "rb");
    if(!file)
        throw std::runtime_error("Unable to open binary file: " + std::string(source));
    
    return File::FilePtr(file, std::fclose);
}

File::FilePtr File::CreateBinaryFile(const char* file_name)
{
    std::FILE* file = std::fopen(file_name, "wb");
    if(!file)
        throw std::runtime_error("Unable to create binary file: " + std::string(file_name));

    return File::FilePtr(file, std::fclose);
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

