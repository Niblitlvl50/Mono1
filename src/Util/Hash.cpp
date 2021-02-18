
#include "Hash.h"
#include <unordered_map>
#include <string>

namespace
{
    std::unordered_map<uint32_t, std::string> g_hash_register;
}

void mono::HashRegisterString(const char* string)
{
    g_hash_register[mono::Hash(string)] = string;
}

const char* mono::HashLookup(uint32_t hash_value)
{
    const auto it = g_hash_register.find(hash_value);
    if(it != g_hash_register.end())
        return it->second.c_str();

    return "unknown";
}
