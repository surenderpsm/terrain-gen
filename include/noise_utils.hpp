#pragma once

#include <FastNoiseLite.h>

float fbm_noise(FastNoiseLite& noise, float x, float y, int octaves = 4, float persistence = 0.5f) {
    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxAmplitude = 0.0f;

    for (int i = 0; i < octaves; ++i) {
        total += noise.GetNoise(x * frequency, y * frequency) * amplitude;
        maxAmplitude += amplitude;
        amplitude *= persistence;
        frequency *= 2.0f;
    }

    return total / maxAmplitude;
}
