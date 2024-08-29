#pragma once

#include <string>

class Base64Helper {
public:
    static std::string Encode(const std::string& data);
    static std::string Decode(const std::string& encoded);
};


