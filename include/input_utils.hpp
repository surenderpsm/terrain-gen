
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Camera {
    glm::vec3 position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 front = { 0.0f, 0.0f, -1.0f };
    glm::vec3 up = { 0.0f, 1.0f, 0.0f };
    float fov = 45.0f;

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 getProjectionMatrix(float aspectRatio) const {
        return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 3000.0f);
    }
};


struct Material {
    glm::vec3 diffuse = { 0.9f, 0.85f, 0.9f };
    glm::vec3 specular = { 0.6f, 0.6f, 0.6f };
    float shininess = 32.0f;
};

struct Light {
    glm::vec3 position = { 256.0f, 300.0f, 256.0f };
    glm::vec3 color = { 1.0f, 1.0f, 1.0f };
};

struct FBMParams {
    float frequency = 0.1f;
    int octaves = 4;
    float persistence = 0.5f;
};
