
#pragma once

#include <memory>
#include <vector>

using byte = unsigned char;

namespace File
{
    //! A smart file handle
    using FilePtr = std::unique_ptr<FILE, int(*)(FILE*)>;

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
