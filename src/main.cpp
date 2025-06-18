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
        if (avg > 0.6f) return ZoneType::HIGH_GROUND; //PURPLE
        if (avg < -0.4f) return ZoneType::CHOKE; // RED
        if ((x < MAP_WIDTH / 4 || x > 3 * MAP_WIDTH / 4) ||
            (y < MAP_HEIGHT / 4 || y > 3 * MAP_HEIGHT / 4)) return ZoneType::FLANK; // BLUE
        return ZoneType::ARENA; // GREEN
    };

    auto fullStrategy = [&](float avg, float stddev, int x, int y) -> ZoneType {
        // Objectives near the center
        if ((x > MAP_WIDTH / 2 - 16 && x < MAP_WIDTH / 2 + 16) &&
            (y > MAP_HEIGHT / 2 - 16 && y < MAP_HEIGHT / 2 + 16))
            return ZoneType::OBJECTIVE;

        // High ground by height
        if (avg > 0.6f)
            return ZoneType::HIGH_GROUND;

        // Choke points: valleys with low deviation (traps)
        if (avg < -0.4f && stddev < 0.05f)
            return ZoneType::CHOKE;

        // Flanks: near map edges
        if ((x < MAP_WIDTH / 6 || x > 5 * MAP_WIDTH / 6) ||
            (y < MAP_HEIGHT / 6 || y > 5 * MAP_HEIGHT / 6))
            return ZoneType::FLANK;

        // Spawn zones in corners
        if ((x < MAP_WIDTH / 8 && y < MAP_HEIGHT / 8) ||
            (x > 7 * MAP_WIDTH / 8 && y > 7 * MAP_HEIGHT / 8))
            return ZoneType::SPAWN;

        // Default combat zone
        return ZoneType::ARENA;
        };


    float minf = 0.001f, maxf = 0.5, inc = 0.0001f;
    for (int index = 0; index < static_cast<int>((maxf - minf) / inc); ++index) {
        float FREQUENCY = minf + index * inc;
        run_pipeline(index, MAP_WIDTH, MAP_HEIGHT, ZONE_SIZE, basicStrategy, FREQUENCY, output_dir);
    }

    return 0;
}