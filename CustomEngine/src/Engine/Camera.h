#pragma once

#include <glm/vec2.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat3x3.hpp> // glm::mat4
#include <glm/mat4x4.hpp> // glm::mat4

#include <vector>
#include "BasicGameObject.h"
#include"SceneNode.h"



class Camera : public BasicGameObject
{
private:

    float m_zNear; 
    float m_zFar; 
    float m_width; 
    float m_height; 
    float m_fov; 

    glm::vec3 m_target;
    glm::vec3 m_direction; 
    glm::vec3 m_right; 
    glm::vec3 m_up; 

    glm::mat4 m_view; 
    glm::mat4 m_projection;

public:

    Camera(); 
    Camera(SceneNode * parent, const glm::vec3& position, std::string tag = "Camera");

    ~Camera(); 


    void setzFar(float far)   { m_zFar = far;  }
    void setzNear(float near) { m_zNear = near;  }
    float getzFar() const { return m_zFar; }
    float getzNear() const { return m_zNear; }
    void setFov(float fov) { m_fov = fov;  }
    float getFov() const { return m_fov; }

    void setTargetPoint(const glm::vec3& target); 
    glm::vec3 getTargetPoint() const { return m_target;  }

    void setRight();
    void setUp();

    void setLookAt();
    glm::mat4 getLookAt() const { return m_view; }

    void setPerspective(float zfar, float znear, float w, float h, bool ortho=false); 
    glm::mat4 getPerspective() const { return m_projection;  }

    void move(const glm::vec3& direction, float dmove); 

    bool hasMesh() override { return false;  }

    void Update(float deltatime) override {}; 
    void print() override { 
        std::cout << "[Camera " << m_id << "] Position " << 
                        m_position.x << ","<<  m_position.y << ","
                        << m_position.z << ".";  }

};

