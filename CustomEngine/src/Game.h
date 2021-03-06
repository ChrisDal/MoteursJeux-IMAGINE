#pragma once


// --------------------------------------
// Backend / Display
#include <glad/glad.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>
// ---------------------------------- // 
// STL
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>

// ---------------------------------- // 
// GLM 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ---------------------------------- // 
// Space Engine : Rendering 
#include "Engine/Rendering/Rendering.h"
// ---------------------------------- // 
// Space Engine : Objects 
#include "Engine/SceneNode.h"
#include "Engine/GameObject.hpp"
#include "Engine/ThirdPersonCamera.h"
#include "Engine/Light/LightObject.h"
#include "Engine/GameObjects/StarBox.h"
// ---------------------------------- // 


/**
 * \file Game.h
 * \brief Game Application. 
 * \version 0.1
 *
 * Programme principal qui g?re le d?roulement du jeu et le chargement des ressources. 
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
    std::unique_ptr<Texture> boxDiffuse;
    std::unique_ptr<Texture> boxSpecular;
        
    std::string m_texturedir;           /*!< Texture Directory */
    std::string m_datadir;              /*!< Data Directory */

    // Renderer 
    Renderer m_renderer;                    /*!< Graphics Renderer */

    // Scene
    SceneNode* m_scene;                     /*!< Current Scene */
    ThirdPersonCamera* m_camera; // owned by scene     /*!< Camera owned by the scene */
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
    void DisplayUISceneGraph(SceneNode* root, int& selected);
    void initWindow();                  //!< Init GLFW Window and ImGui

    // Camera handling 
    void handleInput(GLFWwindow* window, GameObject* actor, Camera* cam);
    void setTrackingPoint(GLFWwindow* window, Camera* cam, double& x, double& z);


protected : 
    // Scene 
    void initScene();                           //!< Init Scene with its GameObjects
    void initScene(const char* filename);       //!< Future initScene with file 
    void saveScene(const char* filename);       //!< Future saveScene in file 

    SceneNode* addSatellite(SceneNode* planetNode, const glm::vec3& satpos,
        float scale = 1.0f,
        const glm::vec3& rotationInDegree = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3& translation = glm::vec3(0.0f, 0.0f, 0.0f)); 

    SceneNode* addPlanet(SceneNode* root, const glm::vec3& nodepos,
        const glm::vec3& planetpos,
        float scale = 1.0f, const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3& translation = glm::vec3(0.0f, 0.0f, 0.0f)); 

    void createSpace(SceneNode* node, float radius, int starsNumbers); 

private:
    void processInput(GLFWwindow* window, bool internal=false);      //!< Process Keyboard Inputs 
    void retrievePlayer(SceneNode* root);       //!< Utility function Retrieve GameObject Player
    GLFWwindow* m_Window;                       /*!< GLFW Window */

    // Camera 
    const glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
    const glm::vec3 m_back{ 0.0f, -1.0f, 0.0f };
    const glm::vec3 m_left{ -1.0f, 0.0f, 0.0f };
    const glm::vec3 m_right{ 1.0f, 0.0f, 0.0f };
    const glm::vec3 m_zpos{ 0.0f, 0.0f,  1.0f };
    const glm::vec3 m_zneg{ 0.0f, 0.0f, -1.0f };

    float sensitivity = 0.02f;              /*!< Mouse Sensitivity */

    bool mouse_left_down;
    bool mouse_right_down;
};

