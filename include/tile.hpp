#pragma once
#include <string>

enum class ZoneType {
    UNASSIGNED,
    SPAWN,
    ARENA,
    CHOKE,
    FLANK,
    HIGH_GROUND,
    OBJECTIVE
};

struct Tile {
    float height = 0.0f;
    bool walkable = true;
    bool has_cover = false;
    ZoneType zone_type = ZoneType::UNASSIGNED;
};
