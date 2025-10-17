#pragma once
#include <string>

struct Company {
    std::string name;
    std::string logo;

    Company(const std::string& n, const std::string& l) : name(n), logo(l){
        
    }
};