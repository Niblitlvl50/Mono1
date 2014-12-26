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
    //! A smart file handle
    typedef std::shared_ptr<FILE> FilePtr;

    //! Opens a binary file
    FilePtr OpenBinaryFile(const std::string& source);

    //! Calculates the size of a file
    //! \param file Input file
    //! \return Size of the file
    long FileSize(const FilePtr file);

    //! Read the file into a buffer
    //! \param file The file to read from
    //! \param bytes Buffer that will hold the data
    void FileRead(FilePtr file, std::vector<byte>& bytes);
}
