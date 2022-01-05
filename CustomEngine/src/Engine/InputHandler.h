#pragma once 

#include "GameObject.hpp"
#include "Camera.h"

class InputHandler
{
public : 
	InputHandler(GLFWwindow* window); 
	~InputHandler() {}; 
	void handleInput(GameObject* actor, Camera* cam);

	void setTrackingPoint(Camera* cam, double& x, double& z);


private: 
	GLFWwindow* window; 
	glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
	glm::vec3 m_back{ 0.0f, -1.0f, 0.0f };
	glm::vec3 m_left{ -1.0f, 0.0f, 0.0f };
	glm::vec3 m_right{ 1.0f, 0.0f, 0.0f };

	bool mouse_left_down = false; 
	bool mouse_right_down = false; 
};
