#include "terrain_renderer.hpp"
#include "shader.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>



TerrainRenderer::TerrainRenderer(TileMap& map) : map(UpdateableMap(map)) {
   
	// create VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

	// Bind VAO, VBO, and EBO
    glBindVertexArray(VAO);

	// Set vertex attributes pointers position and normal
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0); 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);


    std::cout << "[TerrainRenderer] Compiling shader...\n";
    shader = createShaderProgram("resources/shaders/terrain.vert", "resources/shaders/terrain.frag");

    if (!shader) {
        std::cerr << "[TerrainRenderer][ERROR] Shader program failed to compile or link.\n";
    }
    else {
        std::cout << "[TerrainRenderer] Shader loaded successfully.\n";
    }

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
}

void TerrainRenderer::draw(const Material& material, const Light& light) {
    if (!shader || vertices.size() == 0) {
        std::cerr << "[TerrainRenderer][WARN] Skipping draw — invalid shader or no vertices.\n";
        return;
    }

    glUseProgram(shader);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(256, 150, 600), glm::vec3(256, 0, 256), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 3000.0f);
    glm::vec3 lightPos = light.position;
    glm::vec3 viewPos = glm::vec3(256.0f, 150.0f, 600.0f);
    glm::vec3 lightColor = light.color;


    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(viewPos));


  
    // Rebind VAO and draw elements
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}

void TerrainRenderer::draw() {
    if (!shader || vertices.size() == 0) {
        std::cerr << "[TerrainRenderer][WARN] Skipping draw — invalid shader or no vertices.\n";
        return;
    }

    glUseProgram(shader);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(256, 150, 600), glm::vec3(256, 0, 256), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 3000.0f);
    glm::vec3 lightPos = glm::vec3(200.0f, 500.0f, 200.0f);
    glm::vec3 viewPos = glm::vec3(256.0f, 150.0f, 600.0f);
	glm::vec3 lightColor = glm::vec3(4.0f, 3.0f, 1.0f);
	glm::vec3 objectColor = glm::vec3(0.5f, 0.2f, 0.5f);


    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(viewPos));

	glUniform3fv(glGetUniformLocation(shader, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(shader, "objectColor"), 1, glm::value_ptr(objectColor));


	// Rebind VAO and draw elements
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}
void TerrainRenderer::compute() {
    vertices.clear();
    indices.clear();
    int width = map.getWidth();
    int height = map.getHeight();
    std::cout << "[TerrainRenderer] Updating TileMap size: " << width << "x" << height << std::endl;
    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            float y = map.getTileHeight(x, z) * 20.0f; // @todo: make this configurable
            Vertex vertex;
            vertex.position = { static_cast<float>(x), y, static_cast<float>(z) };
            vertex.normal = glm::vec3(0.0f); // Placeholder normal, will be calculated later by computeNormals
            vertices.push_back(vertex);
        }
    }

    computeTriangleIndices(indices, vertices, height, width);
    int ver

    computeNormals(vertices, indices);
}
void TerrainRenderer::updateMesh(const FBMParams& fbm) {
	map.update(fbm);
    vertices.clear();
    indices.clear();
    int width = map.getWidth();
    int height = map.getHeight();
    std::cout << "[TerrainRenderer] Updating TileMap size: " << width << "x" << height << std::endl;
    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            float y = map.getTileHeight(x, z) * 20.0f; // @todo: make this configurable
            Vertex vertex;
            vertex.position = { static_cast<float>(x), y, static_cast<float>(z) };
            vertex.normal = glm::vec3(0.0f); // Placeholder normal, will be calculated later by computeNormals
            vertices.push_back(vertex);
        }
	}

    computeTriangleIndices(indices, vertices, height, width);
    int vertexCount = vertices.size();
    int indexCount = indices.size();

    computeNormals(vertices, indices);

    if (vertices.size() == 0) {
        std::cerr << "[TerrainRenderer][ERROR] No vertices generated. TileMap might be empty or corrupted.\n";
        return;
    }
    else {
        std::cout << "[TerrainRenderer] Generated " << vertexCount << " vertices\n";
    }
    if (indices.size() == 0) {
        std::cerr << "[TerrainRenderer][ERROR] No indices generated. Check the TileMap data.\n";
        return;
    }
    else {
        std::cout << "[TerrainRenderer] Generated " << indexCount / 3 << " triangles\n";
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Upload vertex data to VBO and index data to EBO
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

}

void TerrainRenderer::updateShader(const Material& material, const Light& light) {
    if (!shader) {
        std::cerr << "[TerrainRenderer][ERROR] Shader program is not initialized.\n";
        return;
    }
    glUseProgram(shader);
    glm::vec3 lightPos = light.position;
    glm::vec3 viewPos = glm::vec3(256.0f, 150.0f, 600.0f);
    glm::vec3 lightColor = light.color;
    glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(viewPos));
    glUniform3fv(glGetUniformLocation(shader, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform1f(glGetUniformLocation(shader, "material.shininess"), material.shininess);
    glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, glm::value_ptr(material.diffuse));
    glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, glm::value_ptr(material.specular));
}