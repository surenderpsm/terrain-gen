#include "zone.hpp"
#include "FastNoiseLite.h"

void Zone::generate(TileMap& map) {
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.05f);  // Tune this based on map size
    noise.SetSeed(static_cast<int>(type) * 1337 + x_start * 17 + y_start * 29); // For reproducibility

    for (int y = y_start; y < y_start + size; ++y) {
        for (int x = x_start; x < x_start + size; ++x) {
            Tile& t = map.get(x, y);

            float h = noise.GetNoise((float)x, (float)y);  // [-1, 1]
            h = (h + 1.0f) * 0.5f;  // Normalize to [0, 1]

            t.zone_type = type;
            t.height = h;
            t.has_cover = (h > 0.65f);  // Adjust threshold as needed
        }
    }
}
