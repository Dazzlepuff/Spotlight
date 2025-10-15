#pragma once
#include <functional>

struct CubeCoord {
    int x, y, z;
    CubeCoord(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}

    bool operator==(const CubeCoord& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    CubeCoord operator+(const CubeCoord& other) const {
        return CubeCoord(x + other.x, y + other.y, z + other.z);
    }
};

struct CubeCoordHash {
    std::size_t operator()(const CubeCoord& c) const noexcept {
        std::size_t h1 = std::hash<int>{}(c.x);
        std::size_t h2 = std::hash<int>{}(c.y);
        std::size_t h3 = std::hash<int>{}(c.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
