#pragma once
#include "tilemap.hpp"

class TerrainRenderer {
public:
    void initFromTileMap(const TileMap& map);
    void draw();

private:
    unsigned int VAO, VBO, EBO;
    int vertexCount = 0, indexCount = 0;
};
