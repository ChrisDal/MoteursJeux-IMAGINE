#pragma once
#include <glad/glad.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include "ShaderProgram.h"

class Game
{
public:
    ShaderProgram* shaderProgram; 
    std::string m_shaderdir; 
    unsigned int VAO; 
    unsigned int VBO; 
    unsigned int EBO; 

    std::string m_texturedir; 
   
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

