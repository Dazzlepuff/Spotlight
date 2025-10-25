#pragma once
#include <filesystem>
#include <string>

namespace PathUtils {
    std::filesystem::path getExecutableDir();
    std::filesystem::path getConfigPath(const std::string& relativeFile = "");
    std::filesystem::path getAssetPath(const std::string& relativeFile = "");
}
