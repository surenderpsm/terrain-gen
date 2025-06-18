#pragma once
#include <vector>
#include <string>
#include "tile.hpp"

class TileMap {
public:
    TileMap(int width, int height);
    Tile& get(int x, int y);
    void exportZoneMapAsPNG(const std::string& path);
	void exportTerrainMapAsPNG(const std::string& path);
private:
    int width_, height_;
    std::vector<std::vector<Tile>> tiles_;
};