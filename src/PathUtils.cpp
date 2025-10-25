#include "PathUtils.hpp"

#ifdef _WIN32
#include <windows.h>
std::filesystem::path PathUtils::getExecutableDir() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path();
}
#else
#include <unistd.h>
std::filesystem::path PathUtils::getExecutableDir() {
    char buffer[1024];
    ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer));
    return std::filesystem::path(std::string(buffer, count)).parent_path();
}
#endif

std::filesystem::path PathUtils::getConfigPath(const std::string& relativeFile) {
    auto base = getExecutableDir().parent_path() / "config";
    return relativeFile.empty() ? base : base / relativeFile;
}

std::filesystem::path PathUtils::getAssetPath(const std::string& relativeFile) {
    auto base = getExecutableDir().parent_path() / "assets";
    return relativeFile.empty() ? base : base / relativeFile;
}
