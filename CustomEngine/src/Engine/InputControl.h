#pragma once 

#include <glm/vec3.hpp>  

#include "GameObject.hpp"
#include "Camera.h"





class InputControl
{
public : 
	InputControl();
	~InputControl() {};
	void handleInput(GLFWwindow*  window, GameObject* actor, Camera* cam);

	void setTrackingPoint(GLFWwindow* window, Camera* cam, double& x, double& z);


private: 
	glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
	glm::vec3 m_back{ 0.0f, -1.0f, 0.0f };
	glm::vec3 m_left{ -1.0f, 0.0f, 0.0f };
	glm::vec3 m_right{ 1.0f, 0.0f, 0.0f };

	bool mouse_left_down; 
	bool mouse_right_down; 
};
