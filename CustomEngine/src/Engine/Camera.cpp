#include "Camera.h"



Camera::Camera()
{
	velocity.setId(getId()); 
	velocity.setVelocity(2.5f, 1.0f, 2.5f); 
}

Camera::Camera(SceneNode* parent, const glm::vec3& position,
	std::string tag)
	: BasicGameObject(parent, position, tag),
	m_zNear(0.1f), m_zFar(100.f),
	m_target(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_projection(1.0f), m_fov(45.f)
{
	setTargetPoint(m_target); 
	velocity.setId(getId());
	velocity.setVelocity(2.5f, 1.0f, 2.5f);
}

Camera::~Camera()
{

}

void Camera::setTargetPoint(const glm::vec3& target)
{
	m_target = target; 
	m_direction = glm::normalize(m_position - m_target);
	this->setRight(); 
	this->setUp();
	this->setLookAt();
}

void Camera::setRight()
{
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = glm::normalize(glm::cross(up, m_direction)); 

}

void Camera::setUp()
{
	m_up = glm::normalize(glm::cross(m_direction, m_right));
}


void Camera::setLookAt()
{
	glm::vec4 worldposition = this->getTransformationAllIn() * glm::vec4(m_position, 1.0f);
	glm::vec3 posWorld = glm::vec3(worldposition.x, worldposition.y, worldposition.z); 
	m_view = glm::lookAt(posWorld, m_target, m_up); 
}

void Camera::setPerspective(float zfar, float znear, float w, float h, bool ortho)
{
	m_zNear = znear; 
	m_zFar = zfar; 
	m_width = w; 
	m_height = h; 

	//-----------------
	// Projection Matrix 
	//-----------------
	glm::mat4 projection(1.0f);
	float ratio = m_width / m_height;

	if (ortho) // Orthographic Camera 
	{
		m_projection = glm::ortho(0.0f, m_width, 0.0f, m_height, m_zNear, m_zFar);
	}
	else // Use perspective 
	{
		m_projection = glm::perspective(glm::radians(m_fov), ratio, m_zNear, m_zFar);
	}

}

void Camera::move(const glm::vec3& direction, float dmove)
{
	m_position += dmove * direction;
	m_direction = glm::normalize(m_position - m_target);
	this->setRight();
	this->setUp();
	this->setLookAt(); 
}