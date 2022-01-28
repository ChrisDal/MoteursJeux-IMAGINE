#include "Camera.h"





Camera::Camera()
	: m_zNear(0.1f), m_zFar(100.f),
	m_target(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_projection(1.0f), m_fov(45.f), 
	m_walking(false), m_yaw(-90.f), m_pitch(0.0f)
{
	setTargetPoint(m_target);
	velocity.setId(getId()); 
	velocity.setVelocity(0.025f, 0.01f, 0.025f);
}

Camera::Camera(SceneNode* parent, const glm::vec3& position,
	std::string tag)
	: BasicGameObject(parent, position, tag),
	m_zNear(0.1f), m_zFar(100.f),
	m_target(position + glm::vec3(0.0f, 0.0f, -1.0f)),
	m_projection(1.0f), m_fov(45.f), 
	m_walking(false), m_yaw(-90.f), m_pitch(0.0f)
{
	setTargetPoint(m_target); 
	velocity.setId(getId());
	velocity.setVelocity(0.015f, 0.01f, 0.015f);

}


/*Camera::Camera(BasicGameObject* parent, const glm::vec3& position,
	std::string tag)
	: BasicGameObject(parent, position, tag),
	m_zNear(0.1f), m_zFar(100.f),
	m_target(position + glm::vec3(0.0f, 0.0f, -1.0f)),
	m_projection(1.0f), m_fov(45.f),
	m_walking(false), m_yaw(-90.f), m_pitch(0.0f)
{
	setTargetPoint(m_target);
	velocity.setId(getId());
	velocity.setVelocity(0.015f, 0.01f, 0.015f);

}*/



Camera::~Camera()
{

}

/// <summary>
/// Return World position 
/// Doesn't take into account rotation and scaling 
/// </summary>
/// <returns></returns>
glm::vec4 Camera::getWorldPosition()
{
	// Get total transformation from parent 
	glm::mat4 parentTransfo = this->getNode()->getParent()->getMatTotalNodeTransform();
	// Remove Rotation and Scale from parent : 
	// Camera is not under scale or rotation of parent just translation (position)
	SpaceEngine::setTranslationOnly(parentTransfo);

	// Camera Transformation : parent(Translation) * NodeCamera(Transform) * Cam(Transform)
	glm::mat4 ourworldTransform = parentTransfo * this->getNode()->getMatNodeTransform();
	// Basic World Position
	return ourworldTransform * glm::vec4(m_position, 1.0f);
}

void Camera::Update(float deltatime) {

	/*SimulateWalking(0.015f, static_cast<float>(glfwGetTime()));
	movements.move(this, deltatime);*/

	// set Matrix
	move(glm::vec3(0.0f), 0.0f); 

};

void Camera::setTargetPoint(const glm::vec3& target)
{
	m_target = target; 

	m_direction = glm::normalize(glm::vec3(getWorldPosition()) - m_target);
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

	glm::vec3 posWorld = glm::vec3(getWorldPosition());
	m_view = glm::lookAt(posWorld, posWorld - m_direction, m_up);
}

void Camera::setPerspective(float znear, float zfar, float w, float h, bool ortho)
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
		m_zNear = znear > 0.0f ? -znear : znear;
		m_zFar = zfar > 0.0f ? zfar : -zfar;
		m_projection = glm::ortho<float>(0.0f, m_width, 0.0f, m_height, m_zNear, m_zFar);
	}
	else // Use perspective 
	{
		m_projection = glm::perspective<float>(glm::radians(m_fov), ratio, m_zNear, m_zFar);
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

void Camera::updateVectors()
{
	glm::vec3 frontVec; 
	frontVec.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)); 
	frontVec.y = sin(glm::radians(m_pitch));
	frontVec.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_direction = glm::normalize(frontVec); 

	this->setRight();
	this->setUp();
	this->setLookAt();

}

void Camera::processMovement(float dx, float dy)
{
	
	m_yaw += dx * m_sensitivity;
	m_pitch += dy * m_sensitivity;

	if (m_pitch > 89.f) {
		m_pitch = 89.f; 
	}
	if (m_pitch < -89.f) {
		m_pitch = -89.f;
	}

	updateVectors(); 

}


void Camera::SimulateWalking(float intensity, float dt)
{

	if (m_walking)
	{
		glm::vec3 targetP = m_target + m_up * (cos(2.0f * dt) * intensity / 10.0f) +
			m_right * (sin(dt) * intensity); 
		setTargetPoint(targetP);
	}
}