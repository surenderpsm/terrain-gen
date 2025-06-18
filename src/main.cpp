#include <iostream>
#include <vector>
#include "tile.hpp"
#include "zone.hpp"
#include "zone_planner.hpp"
#include "tilemap.hpp"
#include <filesystem>

void run_pipeline(
    const int index,
    const int MAP_WIDTH,
    const int MAP_HEIGHT,
    const int ZONE_SIZE,
    ZoneStrategy strategy,
    float frequency,
    std::string output_dir
) {
    // Initialize the tile map with specified dimensions
    TileMap tileMap(MAP_WIDTH, MAP_HEIGHT);

    // Generate random terrain for the tile map
    tileMap.generateGlobalHeightMap(frequency);

    // Plan the zones based on the generated terrain and strategy
    ZonePlanner planner(MAP_WIDTH, MAP_HEIGHT, ZONE_SIZE);
    auto zones = planner.planZones(tileMap, strategy);

    // TileMap applies the zone
    tileMap.applyZones(zones);

    // Generate unique filenames
    std::string zoneFile = output_dir + "/zone_map_" + std::to_string(index) + ".png";
    std::string terrainFile = output_dir + "/terrain_map_" + std::to_string(index) + ".png";
    std::string overlayFile = output_dir + "/overlay_map_" + std::to_string(index) + ".png";


    // Export maps
    tileMap.exportZoneMapAsPNG(zoneFile);
    tileMap.exportTerrainMapAsPNG(terrainFile);
    tileMap.exportOverlayTerrainMap(overlayFile);

    // Log status
    std::cout << "[Run " << index << "] Frequency: " << frequency << std::endl;
    std::cout << "  - Saved zone map     -> " << zoneFile << std::endl;
    std::cout << "  - Saved terrain map  -> " << terrainFile << std::endl;
    std::cout << "  - Saved overlay map  -> " << overlayFile << std::endl << std::endl;
}


int main() {
    constexpr int MAP_WIDTH = 256;
    constexpr int MAP_HEIGHT = 256;
    constexpr int ZONE_SIZE = 16;

    // create output directory
    std::string output_dir = "dump";
    std::filesystem::create_directories(output_dir);


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

    float minf = 0.001f, maxf = 0.5, inc = 0.0001f;
    for (int index = 0; index < static_cast<int>((maxf - minf) / inc); ++index) {
        float FREQUENCY = minf + index * inc;
        run_pipeline(index, MAP_WIDTH, MAP_HEIGHT, ZONE_SIZE, basicStrategy, FREQUENCY, output_dir);
    }

    return 0;
}