#pragma once


#include <glad/glad.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <ctime>
#include <chrono>

#include "Engine/Rendering/Rendering.h"

#include "Engine/SceneNode.h"
#include "Engine/Camera.h"




class Game
{
public:
    ShaderProgram* shaderProgram; 
    std::string m_shaderdir; 
    VertexArrayBuffer* VAO;
    VertexBuffer* VBO;
    IndexBuffer* EBO; 


    // Texture 
    Texture* boxTexture; 
    std::unique_ptr<Texture> faceTexture;

    std::string m_texturedir; 
    std::string m_datadir; 

    // Material 
    Material material; 

    // Renderer 
    Renderer m_renderer; 

    // Scene
    SceneNode* m_scene;
    Camera* m_camera; // owned by scene 
    static float camoffsetx, camoffsety; 
    BasicGameObject* m_player = nullptr;

    // Controllers 
    // Removed for now
    
    // Time Clock Chrono
    std::chrono::time_point<std::chrono::steady_clock> m_lastTime; 
    std::chrono::time_point<std::chrono::steady_clock> m_startTime; 
   
public:
    Game();
    virtual ~Game();

    void RunGameLoop();

protected:
    void Update(float deltaTime);
    //virtual void Render();
    void RenderDebugMenu();
    void initWindow(); 

    // Camera handling 
    void handleInput(GLFWwindow* window, GameObject* actor, Camera* cam);
    void setTrackingPoint(GLFWwindow* window, Camera* cam, double& x, double& z);


protected : 
    // Scene 
    void initScene();
    void initScene(const char* filename); 
    void saveScene(const char* filename); 

    

private:
    void processInput(GLFWwindow* window); 
    void retrievePlayer(SceneNode* root); 
    GLFWwindow* m_Window;

    // Camera 
    const glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
    const glm::vec3 m_back{ 0.0f, -1.0f, 0.0f };
    const glm::vec3 m_left{ -1.0f, 0.0f, 0.0f };
    const glm::vec3 m_right{ 1.0f, 0.0f, 0.0f };
    const glm::vec3 m_zpos{ 0.0f, 0.0f,  1.0f };
    const glm::vec3 m_zneg{ 0.0f, 0.0f, -1.0f };

    float sensitivity = 0.05f; 

    bool mouse_left_down;
    bool mouse_right_down;
};

