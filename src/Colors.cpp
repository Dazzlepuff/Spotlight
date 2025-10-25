#include "Colors.hpp"

const std::vector<std::string> Colors::all = {
    "Red", "Yellow", "Blue", "Green", "Purple", "White", "Gray"
};

bool Colors::isValid(const std::string& color) {
    return std::find(all.begin(), all.end(), color) != all.end();
}
