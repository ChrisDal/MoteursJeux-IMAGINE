#include "InputHandler.h"

void InputHandler::handleInput(GameObject* actor, Camera* cam)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        return; 
    }
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        actor->velocity.move(m_up, actor); 
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        actor->velocity.move(m_back, actor);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        actor->velocity.move(m_right, actor);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        actor->velocity.move(m_left, actor);
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
        std::cout << "Cursor Pos : (" << xpos << "," << ypos << ")\n"; 
        setTrackingPoint(cam, xpos, ypos); 
    }
}

void InputHandler::setTrackingPoint(Camera* cam, double& x, double& z)
{
    glm::vec3 newTargetPoint = glm::vec3(x, 0.0, z); 
    glm::vec3 direction = newTargetPoint - cam->getTargetPoint() ;
    direction.y = 0.0; 
    direction = glm::normalize(direction); 
    
    newTargetPoint = glm::vec3(direction.x * cam->velocity.vx,
                                direction.x * cam->velocity.vy,
                                direction.y * cam->velocity.vz);
    
    cam->setTargetPoint(newTargetPoint); 

}

InputHandler::InputHandler(GLFWwindow* window)
{
    this->window = window; 
}
