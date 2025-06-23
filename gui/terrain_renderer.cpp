#include "terrain_renderer.hpp"
#include "shader.hpp"
#include <glad/glad.h>
#include <iostream>



TerrainRenderer::TerrainRenderer(TileMap& map) : map(UpdateableMap(map)) {
   
	// create VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

	// Bind VAO, VBO, and EBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Set vertex attributes pointers position and normal
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0); 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);


    std::cout << "[TerrainRenderer] Compiling shader...\n";
    shader = createShaderProgram("resources/shaders/terrain.vert", "resources/shaders/terrain.frag");

    if (!shader) {
        std::cerr << "[TerrainRenderer][ERROR] Shader program failed to compile or link.\n";
        exit(1);
    }
    else {
        std::cout << "[TerrainRenderer] Shader loaded successfully.\n";
    }

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);

    compute();
}


// legacy
void TerrainRenderer::draw(const Material& material, const Light& light) {
    if (!shader || vertices.size() == 0) {
        std::cerr << "[TerrainRenderer][WARN] Skipping draw invalid shader or no vertices.\n";
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

void TerrainRenderer::draw(const Camera& camera, int screenWidth, int screenHeight) {
    if (!shader || vertices.empty()) {
        std::cerr << "[TerrainRenderer][WARN] Skipping draw – invalid shader or no vertices.\n";
        return;
    }

    glUseProgram(shader);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix((float)screenWidth / screenHeight);

    // glm::vec3 objectColor = glm::vec3(0.5f, 0.2f, 0.5f);
    glm::vec3 viewPos = camera.position;

    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // light
    glUniform3fv(glGetUniformLocation(shader, "lightColor"), 1, glm::value_ptr(light.color));
    glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(light.position));
    // viewpos is the position of the camera for rendering lighting
    glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(viewPos));

    // material
    glUniform1f(glGetUniformLocation(shader, "material.shininess"), material.shininess);
    glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, glm::value_ptr(material.diffuse));
    glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, glm::value_ptr(material.specular));

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

    computeNormals(vertices, indices);

    int vertexCount = vertices.size();
    int indexCount = indices.size();


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


    // Upload vertex data to VBO and index data to EBO
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

}
void TerrainRenderer::updateMesh(const FBMParams& fbm) {
	map.update(fbm);
    vertices.clear();
    indices.clear();
    int width = map.getWidth();
    int height = map.getHeight();
    
    compute();
}

void TerrainRenderer::updateShader(const Material& material, const Light& light) {
    this->light = light;
    this->material = material;
}