#include "tilemap.hpp"
#include <fstream>

#include <FastNoiseLite.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


TileMap::TileMap(int width, int height)
    : width_(width), height_(height), tiles_(height, std::vector<Tile>(width)) {
}

const Tile& TileMap::get(int x, int y) const{
    return tiles_[y][x];
}

void TileMap::generateGlobalHeightMap(float frequency) {
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(frequency);


    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            Tile& tile = tiles_[y][x];
            tile.height = noise.GetNoise((float)x,(float)y);
        }
    }
}

void TileMap::applyZones(const std::vector<Zone>& zones) {
    for (const auto& zone : zones) {
        for (int y = zone.y_start; y < zone.y_start + zone.size; ++y) {
            for (int x = zone.x_start; x < zone.x_start + zone.size; ++x) {
                if (x >= 0 && x < width_ && y >= 0 && y < height_) {
                    tiles_[y][x].zone_type = zone.type;
                }
            }
        }
    }
}

void TileMap::exportZoneMapAsPNG(const std::string& path) {
    std::vector<uint8_t> image(width_ * height_ * 3); // RGB

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

void TileMap::exportOverlayTerrainMap(const std::string& path) {
    std::vector<uint8_t> image(width_ * height_ * 3); // RGB

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            const Tile& tile = get(x, y);

            // Base grayscale from height
            uint8_t gray = static_cast<uint8_t>((tile.height + 1.0f) * 127.5f);
            auto [zr, zg, zb] = zoneToColor(tile.zone_type);

            // Blend zone color (30%) with terrain gray (70%)
            int idx = (y * width_ + x) * 3;
            image[idx] = static_cast<uint8_t>(0.7f * gray + 0.3f * zr);
            image[idx + 1] = static_cast<uint8_t>(0.7f * gray + 0.3f * zg);
            image[idx + 2] = static_cast<uint8_t>(0.7f * gray + 0.3f * zb);
        }
    }

    stbi_write_png(path.c_str(), width_, height_, 3, image.data(), width_ * 3);
}
