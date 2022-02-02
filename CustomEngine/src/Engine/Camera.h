#pragma once

#include <glm/vec2.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat3x3.hpp> // glm::mat4
#include <glm/mat4x4.hpp> // glm::mat4

#include <vector>
#include <ctime>
#include "BasicGameObject.h"
#include "SceneNode.h"
#include "Components.h"

namespace SpaceEngine {

    static glm::mat4 getTranslationOnly(const glm::mat4& transformation)
    {
        glm::mat4 transfo(1.0f);
        transfo[3][0] = transformation[3][0];
        transfo[3][1] = transformation[3][1];
        transfo[3][2] = transformation[3][2];
        return transfo;
    };

    inline static void setTranslationOnly(glm::mat4& transformation)
    {
        transformation[0] = { 1.0f , 0.0f, 0.0f, 0.0f };
        transformation[1] = { 0.0f , 1.0f, 0.0f, 0.0f };
        transformation[2] = { 0.0f , 0.0f, 1.0f, 0.0f };
    };

    
}; 

class Camera : public BasicGameObject
{
protected:

    float m_zNear; 
    float m_zFar; 
    float m_width; 
    float m_height; 
    float m_fov; 

    glm::vec3 m_target;
    glm::vec3 m_direction; 
    glm::vec3 m_right; 
    glm::vec3 m_up; 
    glm::vec3 m_front; 

    float m_yaw; 
    float m_pitch; 

    glm::mat4 m_view; 
    glm::mat4 m_projection;

    bool m_walking; 

    

public : // Components 
    Velocity velocity; 
    Velocity movements; 

    float m_sensitivity = 0.02f; 

public:

    Camera(); 
    Camera(SceneNode * parent, const glm::vec3& position, std::string tag = "Camera");
    Camera(BasicGameObject * parent, const glm::vec3& position, std::string tag = "Camera");

    ~Camera(); 

    // ----------------------------
    // Camera Attributes 
    // ----------------------------
    void setzFar(float far)   { m_zFar = far;  }
    void setzNear(float near) { m_zNear = near;  }
    float getzFar() const { return m_zFar; }
    float getzNear() const { return m_zNear; }
    void setFov(float fov) { m_fov = fov;  }
    float getFov() const { return m_fov; }

    // ----------------------------
    // Target 
    // ----------------------------
    virtual void setTargetPoint(const glm::vec3& target); 
    glm::vec3 getTargetPoint() const { return m_target;  }

    // ----------------------------
    // Set Vectors Up Right & View Matrix 
    // ----------------------------
    virtual void setRight();
    virtual void setUp();
    virtual void setLookAt();
    glm::mat4 getLookAt() const { return m_view; }

    // ----------------------------
    // Perspective  
    // ----------------------------
    void setPerspective(float znear, float zfar, float w, float h, bool ortho=false);
    glm::mat4 getPerspective() const { return m_projection;  }

    // ----------------------------
    // Movements 
    // ----------------------------
    void move(const glm::vec3& direction, float dmove);
    void updateVectors(); 
    void processMovement(float dx, float dy); 
    virtual glm::vec4 getWorldPosition() override;

    void SimulateWalking(float intensity, float dt);
    inline void setWalking(bool walk) { m_walking = walk;  }
    bool getWalking() const { return m_walking;  }


    bool hasMesh() override { return false;  }

    void Update(float deltatime) override; 


    void print() override { 
        std::cout << "[Camera " << m_id << "] Position " << 
                        m_position.x << ","<<  m_position.y << ","
                        << m_position.z << ".";  }

};

