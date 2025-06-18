#pragma once
#include <vector>
#include "zone.hpp"

class ZonePlanner {
public:
    ZonePlanner(int map_width, int map_height, int zone_size);
    std::vector<Zone> planZones();

private:
    int width_, height_, zone_size_;
};