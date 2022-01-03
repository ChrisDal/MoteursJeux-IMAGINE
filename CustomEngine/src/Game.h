#pragma once
#include <glad/glad.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Engine/Rendering/ShaderProgram.h"
#include "Engine/Rendering/VertexBuffer.hpp"
#include "Engine/Rendering/IndexBuffer.hpp"
#include "Engine/Rendering/VertexArrayBuffer.h"
#include "Engine/Rendering/Texture.h"
#include "Engine/Rendering/Renderer.h"

#include "Engine/SceneNode.h"


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

    // Game Objects 
    GameObject* m_gmo; 
    SceneNode* m_scene;
   
public:
    Game();
    virtual ~Game();

    void RunGameLoop();

protected:
    /*virtual void Update(float deltaTime);
    virtual void Render();
    virtual void RenderDebugMenu();*/

    

private:
    void processInput(GLFWwindow* window); 
    GLFWwindow* m_Window;
};

