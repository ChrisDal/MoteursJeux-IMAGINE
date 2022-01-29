#pragma once
#include <glm/vec2.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat3x3.hpp> // glm::mat4
#include <glm/mat4x4.hpp> // glm::mat4

#include "Camera.h"
#include "GameObject.hpp"
#include "SceneNode.h"

class ThirdPersonCamera : public Camera
{
private:

    GameObject* m_targetobj; 


public: 
    glm::vec2 dist2target = { 5.0f, 15.0f };  // distance min = x , max = y 
    float m_sensitivity = 0.02f;

public : 

    // ----------------------------
    // Constructors and Destructors 
    // ----------------------------
    ThirdPersonCamera();
    ThirdPersonCamera(SceneNode* parent, const glm::vec3& position, std::string tag = "3PCamera");
    ThirdPersonCamera(SceneNode* node, GameObject* target,  
                    const glm::vec3& position, std::string tag = "3PCamera");

    ~ThirdPersonCamera() {};

    // ----------------------------
    // Target 
    // ----------------------------
    void setTargetPoint(const glm::vec3& target) override;

    // ----------------------------
    // Set Vectors & View Matrix 
    // ----------------------------
    void setRight() override;
    void setUp() override;
    void setLookAt() override;

    // ----------------------------
    // Getters 
    // ----------------------------
    glm::mat4 getPerspective() const { return m_projection; }


    glm::vec4 getWorldPosition() override; 

    // ----------------------------
    // Game Object Functions 
    // ----------------------------
    void Update(float deltatime) override;
    void print() override {
        std::cout << "[Camera " << m_id << "] Position " <<
            m_position.x << "," << m_position.y << ","
            << m_position.z << ".";
    }


};

