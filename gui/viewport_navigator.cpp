#include "viewport_navigator.hpp"
#include <glm/gtc/matrix_transform.hpp>

ViewNavigator::ViewNavigator() {
    camera.position = { 256.0f, 150.0f, 600.0f };
    camera.front = glm::normalize(glm::vec3(256.0f, 0.0f, 256.0f) - camera.position);
    camera.up = { 0.0f, 1.0f, 0.0f };
}

void ViewNavigator::handleMouseButton(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            dragging = true;
            double xpos, ypos;
            glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
            lastX = xpos;
            lastY = ypos;
        } else if (action == GLFW_RELEASE) {
            dragging = false;
        }
    }
}

void ViewNavigator::handleScroll(double yoffset) {
    float zoomSpeed = 5.0f; // tune this
    camera.position += camera.front * static_cast<float>(yoffset) * zoomSpeed;
}


void ViewNavigator::handleCursor(double xpos, double ypos) {
    if (!dragging)
        return;

    float dx = static_cast<float>(xpos - lastX);
    float dy = static_cast<float>(ypos - lastY);

    glm::vec3 right = glm::normalize(glm::cross(camera.front, camera.up));

    camera.position -= right * dx * panSpeed;
    camera.position += camera.up * dy * panSpeed;

    lastX = xpos;
    lastY = ypos;
}

void ViewNavigator::setPanSpeed(float speed) {
    panSpeed = speed;
}

Camera& ViewNavigator::getCamera() {
    return camera;
}
