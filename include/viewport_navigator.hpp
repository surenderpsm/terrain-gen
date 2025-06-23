#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <input_utils.hpp>

class ViewNavigator {
public:
    ViewNavigator();
    
    void handleMouseButton(int button, int action);
    void handleCursor(double xpos, double ypos);
    void handleScroll(double yoffset); // for zoom

    void setPanSpeed(float speed);
    void update();  // if needed
    Camera& getCamera();

private:
    Camera camera;

    bool dragging = false;
    double lastX = 0.0, lastY = 0.0;
    float panSpeed = 0.01f;
};
