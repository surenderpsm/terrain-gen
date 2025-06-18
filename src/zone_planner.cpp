#include "zone_planner.hpp"

ZonePlanner::ZonePlanner(int map_width, int map_height, int zone_size)
    : width_(map_width), height_(map_height), zone_size_(zone_size) {
}

std::vector<Zone> ZonePlanner::planZones() {
    std::vector<Zone> zones;
    int rows = height_ / zone_size_;
    int cols = width_ / zone_size_;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            Zone z;
            z.x_start = col * zone_size_;
            z.y_start = row * zone_size_;
            z.size = zone_size_;
			// @todo: this is a placeholder for zone type assignment logic
            z.type = static_cast<ZoneType>((row + col) % 6 + 1); // rotates through types
            zones.push_back(z);
        }
    }

    return zones;
}
