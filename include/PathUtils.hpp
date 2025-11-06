/**
 * @file PathUtils.hpp
 * @brief Utility functions for locating executable, configuration, and asset directories.
 *
 * Provides cross-platform path resolution for accessing the executable directory
 * and related resource folders such as "config" and "assets".
 */

#pragma once
#include <filesystem>
#include <string>

/**
 * @namespace PathUtils
 * @brief Contains helper functions for resolving application file paths.
 */
namespace PathUtils {

    /**
     * @brief Retrieves the directory where the executable is located.
     * @return The path to the executable's directory.
     */
    std::filesystem::path getExecutableDir();

    /**
     * @brief Constructs a path within the "config" directory.
     * @param relativeFile Optional relative file or subpath within "config".
     * @return The full configuration path.
     */
    std::filesystem::path getConfigPath(const std::string& relativeFile = "");

    /**
     * @brief Constructs a path within the "assets" directory.
     * @param relativeFile Optional relative file or subpath within "assets".
     * @return The full asset path.
     */
    std::filesystem::path getAssetPath(const std::string& relativeFile = "");
}
