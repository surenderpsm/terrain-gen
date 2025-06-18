#pragma once
#include <vector>
#include "zone.hpp"
#include "tilemap.hpp"
#include <functional>


class ZonePlanner {
public:

    // a lambda to define heuristics for assigning ZoneTypes
    using ZoneStrategy = std::function<ZoneType(float avg_height, float stddev, int x, int y)>;


    ZonePlanner(int map_width, int map_height, int zone_size);
	// legacy: remove in future versions
    std::vector<Zone> planZones();
	std::vector<Zone> planZones(const TileMap& map, ZoneStrategy strategy);

private:
    int width_, height_, zone_size_;

    float computeAverageHeight(const TileMap& map, int x_start, int y_start);
    float computeHeightStdDev(const TileMap& map, int x_start, int y_start, float mean);
};