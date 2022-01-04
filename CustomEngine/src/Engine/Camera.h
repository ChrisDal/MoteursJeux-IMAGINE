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

public:

    Camera(); 
    Camera(SceneNode * parent, const glm::vec3& position, std::string tag = "Camera");

    ~Camera(); 

    

    
   

};

