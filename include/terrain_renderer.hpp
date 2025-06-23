#pragma once
#include "tilemap.hpp"
#include <geometry_utils.hpp>
#include <input_utils.hpp>


class UpdateableMap {
    public:
		UpdateableMap(TileMap& map) : defaultMap(map) {}
        void set(TileMap& map) {
            defaultMap = map;
		}
        void update(FBMParams params) {
			defaultMap.generateGlobalHeightMap(params.frequency, params.octaves, params.persistence);
        }
		int getWidth() const { return defaultMap.getWidth(); }
		int getHeight() const { return defaultMap.getHeight(); }
		float getTileHeight(int x, int y) const { return defaultMap.get(x, y).height; }

    private:
		TileMap& defaultMap;

};


class TerrainRenderer {
public:
    TerrainRenderer(TileMap& map);
    
	void updateMesh(const FBMParams& fbm);
	void updateShader(const Material& material, const Light& light);

    void draw(const Camera& camera, int screenWidth, int screenHeight);
    // legacy
	void draw(const Material& material, const Light& light);
private:
	UpdateableMap map;
    Material material;
    Light light;
    unsigned int VAO, VBO, EBO, shader;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

    void compute();
};


