#include "Camera.h"



Camera::Camera(SceneNode* parent, const glm::vec3& position, std::string tag)
	: BasicGameObject(parent, position, tag)
{
	
}

Camera::~Camera()
{

}
