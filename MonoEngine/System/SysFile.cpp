//
//  File.cpp
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "SysFile.h"
#include <string>
#include <stdexcept>

File::FilePtr File::OpenBinaryFile(const std::string& source)
{
    std::FILE* file = std::fopen(source.c_str(), "rb");
    if(!file)
        throw std::runtime_error("Unable to open binary file: " + source);
    
    return File::FilePtr(file, std::fclose);
}

long File::FileSize(const FilePtr file)
{
    std::fseek(file.get(), 0, SEEK_END);
	const long size = std::ftell(file.get());
    std::rewind(file.get());
    
    return size;
}

void File::FileRead(FilePtr file, std::vector<byte>& bytes)
{
    const long filesize = File::FileSize(file);
    
    bytes.resize(filesize, 0);
    std::fread(bytes.data(), 1, filesize, file.get());
}

