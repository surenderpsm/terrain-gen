#pragma once
#include "tilemap.hpp"
#include "tile.hpp"

struct Zone {
    ZoneType type;
    int x_start, y_start, size;

    void generate(TileMap& map);
};