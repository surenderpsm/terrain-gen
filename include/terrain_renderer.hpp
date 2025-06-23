#pragma once
#include "tilemap.hpp"
#include <geometry_utils.hpp>

struct Material {
    glm::vec3 diffuse = { 0.9f, 0.85f, 0.9f };
    glm::vec3 specular = { 0.6f, 0.6f, 0.6f };
    float shininess = 32.0f;
};

struct Light {
    glm::vec3 position = { 256.0f, 300.0f, 256.0f };
    glm::vec3 color = { 1.0f, 1.0f, 1.0f };
};

struct Camera {
    glm::vec3 position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 front = { 0.0f, 0.0f, -1.0f };
    glm::vec3 up = { 0.0f, 1.0f, 0.0f };
    float fov = 45.0f;
};

struct FBMParams {
    float frequency = 0.1f;
    int octaves = 4;
    float persistence = 0.5f;
};


class TerrainRenderer {
public:
    TerrainRenderer(TileMap& map);
    
	void updateMesh(const FBMParams& fbm);
	void updateShader(const Material& material, const Light& light);

    void init();
    void draw();
	void draw(const Material& material, const Light& light);
private:
	UpdateableMap map;
    unsigned int VAO, VBO, EBO, shader;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

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
