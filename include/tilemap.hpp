#pragma once
#include <vector>
#include <string>
#include "tile.hpp"
#include "zone.hpp"

inline std::tuple<uint8_t, uint8_t, uint8_t> zoneToColor(ZoneType type) {
    switch (type) {
    case ZoneType::SPAWN:       return { 255, 255, 0 };   // Yellow
    case ZoneType::ARENA:       return { 0, 200, 0 };     // Green
    case ZoneType::CHOKE:       return { 200, 0, 0 };     // Red
    case ZoneType::FLANK:       return { 0, 0, 255 };     // Blue
    case ZoneType::HIGH_GROUND: return { 128, 0, 128 };   // Purple
    case ZoneType::OBJECTIVE:   return { 255, 140, 0 };   // Orange
    default:                    return { 100, 100, 100 }; // Grey
    }
}

class TileMap {
public:
    TileMap(int width, int height);
	const int getHeight() const { return height_; }
	const int getWidth() const { return width_; }
    const Tile& get(int x, int y) const;
    void generateGlobalHeightMap(float frequency = 0.02f, int octaves = 4, float persistence = 0.5f);
    void generateGlobalHeightMap(float frequency=0.02f);
    std::pair<float, float> computeGlobalStats() const;
    void applyZones(const std::vector<Zone>& zones);
    void exportZoneMapAsPNG(const std::string& path);
	void exportTerrainMapAsPNG(const std::string& path);
    void exportOverlayTerrainMap(const std::string& path);
private:
    int width_, height_;
    std::vector<std::vector<Tile>> tiles_;
};