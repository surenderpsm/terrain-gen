#include "tilemap.hpp"
#include <fstream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


TileMap::TileMap(int width, int height)
    : width_(width), height_(height), tiles_(height, std::vector<Tile>(width)) {
}

Tile& TileMap::get(int x, int y) {
    return tiles_[y][x];
}

void TileMap::exportZoneMapAsPNG(const std::string& path) {
    std::vector<uint8_t> image(width_ * height_ * 3); // RGB
    
    auto zoneToColor = [](ZoneType type) -> std::tuple<uint8_t, uint8_t, uint8_t> {
        switch (type) {
            case ZoneType::SPAWN:       return { 255, 255, 0 };   // Yellow
            case ZoneType::ARENA:       return { 0, 200, 0 };     // Green
            case ZoneType::CHOKE:       return { 200, 0, 0 };     // Red
            case ZoneType::FLANK:       return { 0, 0, 255 };     // Blue
            case ZoneType::HIGH_GROUND: return { 128, 0, 128 };   // Purple
            case ZoneType::OBJECTIVE:   return { 255, 140, 0 };   // Orange
            default:                    return { 100, 100, 100 }; // Grey
        }
    };

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            auto [r, g, b] = zoneToColor(tiles_[y][x].zone_type);
            int idx = (y * width_ + x) * 3;
            image[idx] = r;
            image[idx + 1] = g;
            image[idx + 2] = b;
        }
    }

    stbi_write_png(path.c_str(), width_, height_, 3, image.data(), width_ * 3);
}


void TileMap::exportTerrainMapAsPNG(const std::string& path) {
    std::vector<uint8_t> image(width_ * height_);

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            const Tile& t = tiles_[y][x];
            // Normalize height [-1,1] -> [0,255]
            uint8_t val = static_cast<uint8_t>((t.height + 1.0f) * 127.5f);
            image[y * width_ + x] = val;
        }
    }

    stbi_write_png(path.c_str(), width_, height_, 1, image.data(), width_);
}