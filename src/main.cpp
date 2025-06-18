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

    // Define a strategy
    auto basicStrategy = [&](float avg, float stddev, int x, int y) -> ZoneType {
        if (avg > 0.6f) return ZoneType::HIGH_GROUND;
        if (avg < -0.4f) return ZoneType::CHOKE;
        if ((x < MAP_WIDTH / 4 || x > 3 * MAP_WIDTH / 4) ||
            (y < MAP_HEIGHT / 4 || y > 3 * MAP_HEIGHT / 4)) return ZoneType::FLANK;
        return ZoneType::ARENA;
    };

	// Initialize the tile map with specified dimensions
    TileMap tileMap(MAP_WIDTH, MAP_HEIGHT);

	// Generate random terrain for the tile map
    tileMap.generateGlobalHeightMap();

	// Plan the zones based on the generated terrain and strategy
    ZonePlanner planner(MAP_WIDTH, MAP_HEIGHT, ZONE_SIZE);
    auto zones = planner.planZones(tileMap, basicStrategy);

    // TileMap applies the zone
    tileMap.applyZones(zones);

	// view zone distribution
    tileMap.exportZoneMapAsPNG("zone_map.png");
    std::cout << "Zone Map written to zone_map.png\n";

    // view terrain elevation
	tileMap.exportTerrainMapAsPNG("terrain_map.png");
    std::cout << "Grayscale Terrain Map written to terrain_map.png\n";


    return 0;
}