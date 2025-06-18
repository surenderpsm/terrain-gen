#include "zone_planner.hpp"


ZonePlanner::ZonePlanner(int map_width, int map_height, int zone_size) :
    width_(map_width),
    height_(map_height),
    zone_size_(zone_size) {
}

std::vector<Zone> ZonePlanner::planZones(const TileMap& map, ZoneStrategy strategy) {
    std::vector<Zone> zones;

    for (int y = 0; y < height_; y += zone_size_) {
        for (int x = 0; x < width_; x += zone_size_) {
            float avg = computeAverageHeight(map, x, y);
            float stddev = computeHeightStdDev(map, x, y, avg);

            ZoneType type = strategy(avg, stddev, x, y);
            zones.emplace_back(x, y, zone_size_, type);
        }
    }

    return zones;
}



// legacy
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
            z.type = static_cast<ZoneType>((row + col) % 6 + 1);
			// @todo: this is a placeholder for zone type assignment logic
            // rotates through types
            zones.push_back(z);
        }
    }

    return zones;
}

float ZonePlanner::computeAverageHeight(const TileMap& map, int x_start, int y_start) {
    float total = 0.0f;
    int count = 0;

    for (int dy = 0; dy < zone_size_; ++dy) {
        for (int dx = 0; dx < zone_size_; ++dx) {
            int x = x_start + dx;
            int y = y_start + dy;

            if (x < width_ && y < height_) {
                total += map.get(x, y).height;
                ++count;
            }
        }
    }

    return (count > 0) ? (total / count) : 0.0f;
}

float ZonePlanner::computeHeightStdDev(const TileMap& map, int x_start, int y_start, float mean) {
    float variance = 0.0f;
    int count = 0;

    for (int dy = 0; dy < zone_size_; ++dy) {
        for (int dx = 0; dx < zone_size_; ++dx) {
            int x = x_start + dx;
            int y = y_start + dy;

            if (x < width_ && y < height_) {
                float h = map.get(x, y).height;
                variance += (h - mean) * (h - mean);
                ++count;
            }
        }
    }

    return (count > 0) ? std::sqrt(variance / count) : 0.0f;
}
