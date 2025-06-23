#include "terrain_renderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "tilemap.hpp"
#include "zone_planner.hpp"
#include<iostream>
#include <glm/gtc/type_ptr.hpp>


enum UPDATE_TYPE {
    UPDATE_NONE = 0,
    UPDATE_MESH = 1 << 0,
    UPDATE_SHADER = 1 << 1,
    UPDATE_ALL = UPDATE_MESH | UPDATE_SHADER
};

// Globals
int gScreenWidth = 1920;
int gScreenHeight = 1080;
GLFWwindow* gWindow = nullptr;

// Set up GLFW window and set it to current OpenGL context
void initializeGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
	gWindow = glfwCreateWindow(gScreenWidth, gScreenHeight, "Terrain Viewer", NULL, NULL);
    if (!gWindow) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }
	glfwMakeContextCurrent(gWindow);
}


void cleanup() {

}

int main() {
    struct Light light;
	struct Material material;
	struct FBMParams fbm;
	initializeGLFW();


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(gWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Load data + renderer
    constexpr int MAP_WIDTH = 512;
	constexpr int MAP_HEIGHT = 512;
	//constexpr int ZONE_SIZE = 64; // Size of each zone in the tilemap
    TileMap tileMap(MAP_WIDTH, MAP_HEIGHT);
    tileMap.generateGlobalHeightMap(0.02, 4, 0.5);

    /*ZonePlanner planner(MAP_WIDTH, MAP_HEIGHT, ZONE_SIZE);
    auto zones = planner.planZones(tileMap, your_strategy_here);
    tileMap.applyZones(zones);*/

    TerrainRenderer renderer(tileMap);
    renderer.init();



    // Main loop
    while (!glfwWindowShouldClose(gWindow)) {
        int updateFlags = UPDATE_NONE;
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        ImGui::Begin("Texture Controls");
		ImGui::SeparatorText("Light Settings");
		if (ImGui::ColorEdit3("Light Color", glm::value_ptr(light.color))) updateFlags |= UPDATE_SHADER;
        if (ImGui::SliderFloat3("Light Position", glm::value_ptr(light.position), 0.0f, 1024.0f)) updateFlags |= UPDATE_SHADER;

       
		ImGui::SeparatorText("Material Settings");
        if (ImGui::ColorEdit3("Diffuse", glm::value_ptr(material.diffuse))) updateFlags |= UPDATE_SHADER;
        if (ImGui::ColorEdit3("Specular", glm::value_ptr(material.specular))) updateFlags |= UPDATE_SHADER;
        if (ImGui::SliderFloat("Shininess", &material.shininess, 1.0f, 128.0f)) updateFlags |= UPDATE_SHADER;

        ImGui::End();


        int w, h;
        glfwGetFramebufferSize(gWindow, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (updateFlags & UPDATE_SHADER) {
            renderer.updateShader(light, material);
		}
        else if (updateFlags & UPDATE_MESH) {
            renderer.updateMesh(tileMap);
		}
        else if (updateFlags & UPDATE_ALL) {
			renderer.updateAll(tileMap, light, material);   
        }
        renderer.draw(material, light);


        ImGui::Render();
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(gWindow);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(gWindow);
    glfwTerminate();
    return 0;
}
