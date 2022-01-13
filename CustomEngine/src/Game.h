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


/**
 * \file Game.h
 * \brief Game Application. 
 * \version 0.1
 *
 * Programme principal qui gère le déroulement du jeu et le chargement des ressources. 
*/

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
        
    std::string m_texturedir;           /*!< Texture Directory */
    std::string m_datadir;              /*!< Data Directory */

    // Material 
    Material material; 

    // Renderer 
    Renderer m_renderer;                    /*!< Graphics Renderer */

    // Scene
    SceneNode* m_scene;                     /*!< Current Scene */
    Camera* m_camera; // owned by scene     /*!< Camera owned by the scene */
    static float camoffsetx, camoffsety;    /*!< Delta X Y to move camera */
    static glm::vec2 camDeadZone;               /*!< DeadZone Camera */
    BasicGameObject* m_player = nullptr;    /*!< Object Player */

    // Controllers 
    // Removed for now
    
    // Time Clock Chrono
    std::chrono::time_point<std::chrono::steady_clock> m_lastTime; 
    std::chrono::time_point<std::chrono::steady_clock> m_startTime; 
   
public:
    Game();                             //!< Constructor 
    virtual ~Game();                    //!< Virtual Destructor

    void RunGameLoop();                 //!< Run GameLoop in GLFW Window 

protected:
    void Update(float deltaTime);       //!< Game Loop : Update Game state
    //virtual void Render();
    void RenderDebugMenu();             //!< Render ImGui Window 
    void initWindow();                  //!< Init GLFW Window and ImGui

    // Camera handling 
    void handleInput(GLFWwindow* window, GameObject* actor, Camera* cam);
    void setTrackingPoint(GLFWwindow* window, Camera* cam, double& x, double& z);


protected : 
    // Scene 
    void initScene();                           //!< Init Scene with its GameObjects
    void initScene(const char* filename);       //!< Future initScene with file 
    void saveScene(const char* filename);       //!< Future saveScene in file 

    

private:
    void processInput(GLFWwindow* window);      //!< Process Keyboard Inputs 
    void retrievePlayer(SceneNode* root);       //!< Utility function Retrieve GameObject Player
    GLFWwindow* m_Window;                       /*!< GLFW Window */

    // Camera 
    const glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
    const glm::vec3 m_back{ 0.0f, -1.0f, 0.0f };
    const glm::vec3 m_left{ -1.0f, 0.0f, 0.0f };
    const glm::vec3 m_right{ 1.0f, 0.0f, 0.0f };
    const glm::vec3 m_zpos{ 0.0f, 0.0f,  1.0f };
    const glm::vec3 m_zneg{ 0.0f, 0.0f, -1.0f };

    float sensitivity = 0.05f;              /*!< Mouse Sensitivity */

    bool mouse_left_down;
    bool mouse_right_down;
};

