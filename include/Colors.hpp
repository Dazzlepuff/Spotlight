#pragma once
#include <vector>
#include <string>
#include <algorithm>

class Colors {
public:
    static const std::vector<std::string> all;

    static bool isValid(const std::string& color);
};
