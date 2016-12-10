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
    using FilePtr = std::shared_ptr<FILE>;

    //! Opens a binary file for reading
    FilePtr OpenBinaryFile(const char* file_name);

    //! Opens a ascii file for reading
    FilePtr OpenAsciiFile(const char* file_name);

    //! Creates a binary file for writing, will overwrite if already exists
    FilePtr CreateBinaryFile(const char* file_name);

    //! Create an ascii file for writing, will overwrite of already exists
    FilePtr CreateAsciiFile(const char* file_name);

    //! Calculates the size of a file
    //! \param file Input file
    //! \return Size of the file
    long FileSize(const FilePtr& file);

    //! Read the file into a buffer
    //! \param file The file to read from
    //! \param bytes Buffer that will hold the data
    void FileRead(const FilePtr& file, std::vector<byte>& bytes);
}
