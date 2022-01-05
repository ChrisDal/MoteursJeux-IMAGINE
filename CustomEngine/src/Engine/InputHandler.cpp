#include "InputHandler.h"

void InputHandler::handleInput(GameObject* actor, Camera* cam)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        return; 
    }
    
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        actor->velocity.move(m_up, actor); 
        std::cout << "Up Actor - ";
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        actor->velocity.move(m_back, actor);
        std::cout << "BAck Actor - "; 
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        actor->velocity.move(m_right, actor);
        std::cout << "Right Actor - ";
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        actor->velocity.move(m_left, actor);
        std::cout << "Left Actor - ";
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
        setTrackingPoint(cam, xpos, ypos); 
    }
}

void InputHandler::setTrackingPoint(Camera* cam, double& x, double& y)
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

InputHandler::InputHandler(GLFWwindow* window)
{
    this->window = window; 
}
