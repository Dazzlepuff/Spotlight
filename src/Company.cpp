#include "Company.hpp"

// Constructor
Company::Company(const std::string& name, const std::string& symbol)
    : name(name), symbol(symbol) {}

// Getters
const std::string& Company::getName() const {
    return name;
}

const std::string& Company::getSymbol() const {
    return symbol;
}

// Setters
void Company::setName(const std::string& newName) {
    name = newName;
}

void Company::setSymbol(const std::string& newSymbol) {
    symbol = newSymbol;
}
