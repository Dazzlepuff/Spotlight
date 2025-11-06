#include "Company.hpp"

/**
 * @details
 * Constructs the Company with a designated name and symbol. 
 * Typically called during setup or game initialization.
 */
Company::Company(const std::string& name, const std::string& symbol)
    : name(name), symbol(symbol) {}

/** @brief Returns a constant reference to the company’s name. */
const std::string& Company::getName() const {
    return name;
}

/** @brief Returns a constant reference to the company’s identifying symbol. */
const std::string& Company::getSymbol() const {
    return symbol;
}

/**
 * @details
 * Allows dynamic renaming of a company, useful in sandbox or modifiable environments.
 */
void Company::setName(const std::string& newName) {
    name = newName;
}

/**
 * @details
 * Updates the company symbol. Intended for cosmetic or branding adjustments.
 */
void Company::setSymbol(const std::string& newSymbol) {
    symbol = newSymbol;
}
