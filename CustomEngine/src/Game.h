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

#include "Engine/Rendering/ShaderProgram.h"
#include "Engine/Rendering/VertexBuffer.hpp"
#include "Engine/Rendering/IndexBuffer.hpp"
#include "Engine/Rendering/VertexArrayBuffer.h"
#include "Engine/Rendering/Texture.h"
#include "Engine/Rendering/Renderer.h"

#include "Engine/SceneNode.h"
#include "Engine/Camera.h"
#include "Engine/InputHandler.h"


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

    // Renderer 
    Renderer m_renderer; 

    // Scene
    SceneNode* m_scene;
    Camera* m_camera; // owned by scene 
    BasicGameObject* m_player = nullptr;

    // Controllers 
    InputHandler* m_input; 
    
    

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

    

private:
    void processInput(GLFWwindow* window); 
    void retrievePlayer(SceneNode* root); 
    GLFWwindow* m_Window;
};

