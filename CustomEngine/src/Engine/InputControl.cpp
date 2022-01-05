

#include "InputControl.h"
#include "GameObject.hpp"
#include "Camera.h"



InputControl::InputControl()
    : mouse_left_down(false), mouse_right_down(false)
{

}


void InputControl::handleInput(GLFWwindow* window, GameObject* actor, Camera* cam)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        return; 
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        actor->velocity.vy += 5.0f; 
        std::cout << "Up Actor - ";
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        actor->velocity.vy -= 5.0f;
        std::cout << "BAck Actor - "; 
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        actor->velocity.vx += 5.0f;
        std::cout << "Right Actor - ";
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        actor->velocity.vx -= 5.0f;
        
        std::cout << "Left Actor - ";
    }

    if (actor->velocity.ismoving()) {
        cam->setWalking(true);
    }
    else
    {
        cam->setWalking(false);
    }

    // Mouse 
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        mouse_left_down = true; 

    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        mouse_left_down = false;
    }

    if (mouse_left_down)
    {
        double xpos, ypos; 
        glfwGetCursorPos(window, &xpos, &ypos);
        setTrackingPoint(window, cam, xpos, ypos); 
    }

}

void InputControl::setTrackingPoint(GLFWwindow* window, Camera* cam, double& x, double& y)
{
    int w, h; 
    glfwGetWindowSize(window, &w, &h);

    // centered target
    glm::vec3 newTargetPoint = glm::vec3(x, y, 0.0f) - glm::vec3((float)w / 2.0f, (float)h / 2.0f, 0.0f); 
    glm::vec3 oldTargetPoint = cam->getTargetPoint();
    // direction
    glm::vec3 direction = newTargetPoint - oldTargetPoint;
    direction.z = 0.0f; 
    direction = glm::normalize(direction); 
    
    newTargetPoint = oldTargetPoint + glm::vec3(direction.x * cam->velocity.vx,
                                                - direction.y * cam->velocity.vy,
                                                0.0f);

    cam->setTargetPoint(newTargetPoint);

}
