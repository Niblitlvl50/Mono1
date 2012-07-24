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

namespace
{
    void CloseFileDeleter(FILE* file)
    {
        fclose(file);
    }
}

File::FilePtr File::OpenBinaryFile(const std::string& source)
{
    FILE* file = fopen(source.c_str(), "rb");
    if(!file)
        throw std::runtime_error("Unable to open binary file: " + source);
    
    return File::FilePtr(file, CloseFileDeleter);
}

long File::FileSize(const FilePtr file)
{
    fseek(file.get(), 0, SEEK_END);
	const long size = ftell(file.get());    
    rewind(file.get());
    
    return size;
}
