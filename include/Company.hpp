#pragma once
#include <string>

class Company {
public:
    Company(const std::string& name, const std::string& symbol);

    // Getters
    const std::string& getName() const;
    const std::string& getSymbol() const;

    // Setters (optional)
    void setName(const std::string& newName);
    void setSymbol(const std::string& newSymbol);

private:
    std::string name;
    std::string symbol; // the company symbol
};
