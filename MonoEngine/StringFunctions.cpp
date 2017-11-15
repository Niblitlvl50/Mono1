
#include "StringFunctions.h"

std::vector<std::string> mono::SplitString(const std::string& string, char delimiter)
{
    std::vector<std::string> tokens;

    size_t search_position = 0;

    do {
        const size_t pos = string.find_first_of(delimiter, search_position);
        if(pos != std::string::npos)
            tokens.push_back(string.substr(search_position, pos));
        
        search_position = pos;
    } while(search_position != std::string::npos);

    return tokens;
}
