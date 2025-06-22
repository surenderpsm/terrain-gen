#include "terrain_renderer.hpp"
#include "shader.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

unsigned int shaderProgram;

struct Vertex {
    float x, y, z;
};

void TerrainRenderer::initFromTileMap(const TileMap& map) {
    std::vector<Vertex> vertices;

    int width = map.getWidth();
    int height = map.getHeight();

    std::cout << "[TerrainRenderer] TileMap size: " << width << "x" << height << std::endl;

    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            float y = map.get(x, z).height * 20.0f;
            vertices.push_back({ static_cast<float>(x), y, static_cast<float>(z) });
        }
    }

    std::vector<unsigned int> indices;

    for (int z = 0; z < height - 1; ++z) {
        for (int x = 0; x < width - 1; ++x) {
            int topLeft = z * width + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * width + x;
            int bottomRight = bottomLeft + 1;

            // Triangle 1
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Triangle 2
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }


    vertexCount = vertices.size();
    std::cout << "[TerrainRenderer] Generated " << vertexCount << " vertices\n";

    if (vertexCount == 0) {
        std::cerr << "[TerrainRenderer][ERROR] No vertices generated. TileMap might be empty or corrupted.\n";
        return;
    }

	// create VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    indexCount = indices.size();


    std::cout << "[TerrainRenderer] Compiling shader...\n";
    shaderProgram = createShaderProgram("resources/shaders/terrain.vert", "resources/shaders/terrain.frag");

    if (!shaderProgram) {
        std::cerr << "[TerrainRenderer][ERROR] Shader program failed to compile or link.\n";
    }
    else {
        std::cout << "[TerrainRenderer] Shader loaded successfully.\n";
    }

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
}

void TerrainRenderer::draw() {
    if (!shaderProgram || vertexCount == 0) {
        std::cerr << "[TerrainRenderer][WARN] Skipping draw — invalid shader or no vertices.\n";
        return;
    }

    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(256, 150, 600), glm::vec3(256, 0, 256), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 3000.0f);


    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

}
