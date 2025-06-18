#pragma once
#include <vector>
#include <string>
#include "tile.hpp"
#include "zone.hpp"

class TileMap {
public:
    TileMap(int width, int height);
    const Tile& get(int x, int y) const;
    void generateGlobalHeightMap(float frequency=0.02f);
    void applyZones(const std::vector<Zone>& zones);
    void exportZoneMapAsPNG(const std::string& path);
	void exportTerrainMapAsPNG(const std::string& path);
private:
    int width_, height_;
    std::vector<std::vector<Tile>> tiles_;
};