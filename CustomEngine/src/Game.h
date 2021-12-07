#pragma once
#include <glad/glad.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "ShaderProgram.h"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArrayBuffer.h"
#include "Texture.h"


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

