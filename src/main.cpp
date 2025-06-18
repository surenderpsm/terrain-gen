#include <iostream>
#include <vector>
#include "tile.hpp"
#include "zone.hpp"
#include "zone_planner.hpp"
#include "tilemap.hpp"




int main() {
    constexpr int MAP_WIDTH = 256;
    constexpr int MAP_HEIGHT = 256;
    constexpr int ZONE_SIZE = 16;

    TileMap tileMap(MAP_WIDTH, MAP_HEIGHT);
    ZonePlanner planner(MAP_WIDTH, MAP_HEIGHT, ZONE_SIZE);

    auto zones = planner.planZones();

    for (auto& zone : zones) {
        zone.generate(tileMap);
    }

	// view zone distribution
    tileMap.exportZoneMapAsPNG("zone_map.png");
    std::cout << "Zone Map written to zone_map.png\n";

    // view terrain elevation
	tileMap.exportTerrainMapAsPNG("terrain_map.png");
    std::cout << "Grayscale Terrain Map written to terrain_map.png\n";


    return 0;
}