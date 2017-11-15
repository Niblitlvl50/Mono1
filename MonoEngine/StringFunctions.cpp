
#include "StringFunctions.h"

std::vector<std::string> mono::SplitString(const std::string& string, char delimiter)
{
    std::vector<std::string> tokens;

    size_t start_position = 0;
    size_t find_position = std::string::npos;

    do {
        find_position = string.find_first_of(delimiter, start_position);
        if(find_position != std::string::npos)
        {
            tokens.push_back(string.substr(start_position, find_position - start_position));
            start_position = find_position +1;
        }
    } while(find_position != std::string::npos);

    tokens.push_back(string.substr(start_position));

    return tokens;
}
