//
//  File.h
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <memory>
#include <vector>

#include "SysTypes.h"

namespace File
{
    typedef std::shared_ptr<FILE> FilePtr;
    
    FilePtr OpenBinaryFile(const std::string& source);

    long FileSize(const FilePtr file);
    void FileRead(FilePtr file, std::vector<byte>& bytes);
}
