#pragma once

enum class ZoneType;


struct Zone {
    ZoneType type;
    int x_start, y_start, size;

    Zone() {};

    Zone(int x, int y, int size, ZoneType type)
        : x_start(x), y_start(y), size(size), type(type) {
    }
};