#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera()
	: Camera(), m_targetobj(nullptr)
{
	std::cout << "[LOG] Error no target and no node for Camera. Will Not create a 3rd Person Camera\n";
}


/// <summary>
/// Camera Constructor in SceneGraph with no Target
/// Target is set 0,0,0 direction
/// </summary>
/// <param name="parent"> SceneNode where is attached the camera </param>
/// <param name="position">Position in SceneNode Repere</param>
/// <param name="tag"> default to "3PCamera" for third Person Camera </param>
ThirdPersonCamera::ThirdPersonCamera(SceneNode* parent, const glm::vec3& position, std::string tag)
	: Camera(parent, position, tag), m_targetobj(nullptr)
{
	setTargetPoint(m_target, 0.0f);
}

ThirdPersonCamera::ThirdPersonCamera(SceneNode* node, GameObject* target, const glm::vec3& position, std::string tag)
	: Camera(node, position, tag), m_targetobj(target)
{
	// set Object position in world 
	glm::mat4 trfmgmo = m_targetobj->getTransformationAllIn();
	glm::vec4 posgmo = trfmgmo * glm::vec4(m_targetobj->Position(), 1.0f);
	// Very basic follow perso at the center of the screen 
	setTargetPoint(glm::vec3(posgmo), 0.0f);
}


void ThirdPersonCamera::setTargetPoint(const glm::vec3& target, float deltatime)
{
	
	m_target = target;
	// camera position in world 
	glm::vec4 worldposition = this->getWorldPosition(); 
	// Process Distance and direction
	glm::vec3 diff = glm::vec3(worldposition) - m_target;
	float normvec = std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
	m_direction = glm::normalize(diff);

	// Move camerauntil we are in acceptable bounds 
	if (normvec > dist2target.y) {
		m_position -= deltatime * m_direction * m_sensitivity;
	}

	if (normvec < dist2target.x) {
		m_position += deltatime * m_direction * m_sensitivity;
	}


	this->setRight();

	// Vec : Player -> Camera 
	glm::vec3 camPlayer = glm::vec3(worldposition) - m_target;
	// Vec for which cosTheta = 0 // ?? 
	glm::vec3 playerTowards = glm::vec3(-1.0f, 0.0f, 0.0f); 
	float costheta = glm::dot(camPlayer, playerTowards);
	// Move if angle is too wide 
	if (std::fabs(costheta) > m_cosangleCamera) {
		m_position += costheta * deltatime *  m_right * m_sensitivity;
	}



	this->setUp();
	this->setLookAt();

}

void ThirdPersonCamera::setTargetObject(GameObject* obj)
{
	if (obj != nullptr) {
		m_targetobj = obj; 
	}
}

void ThirdPersonCamera::setDistanceToTarget(float mindist, float maxdist)
{
	if (mindist < 0.0f || maxdist < 0.0f)
	{
		std::cout << "Distances have to be positive.\n";
		return; 
	}

	if (mindist < maxdist)
	{
		dist2target = glm::vec2(mindist, maxdist); 
	}
	else if (maxdist < mindist)
	{
		dist2target = glm::vec2(maxdist, mindist);
	}
	else
	{
		std::cout << "Min and Max distances have to be differents.\n"; 
	}
}

void ThirdPersonCamera::setMaxTowardsAngle(float angledegree)
{
	m_cosangleCamera = std::fabs(std::cosf(angledegree)); 
}

void ThirdPersonCamera::setRight()
{
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = glm::normalize(glm::cross(up, m_direction));
}

void ThirdPersonCamera::setUp() 
{
	m_up = glm::cross(m_direction, m_right); 
}

void ThirdPersonCamera::setLookAt()
{
	m_view = glm::lookAt(m_position, m_target, m_up);
}

/*void ThirdPersonCamera::setPerspective(float znear, float zfar, float w, float h, bool ortho)
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

}*/

/// <summary>
/// Return World position 
/// Doesn't take into account rotation and scaling 
/// </summary>
/// <returns></returns>
/*glm::vec4 ThirdPersonCamera::getWorldPosition()
{
	// Get total transformation from parent 
	glm::mat4 parentTransfo = this->getNode()->getParent()->getMatTotalNodeTransform();
	// Remove Rotation and Scale from parent : 
	// Camera is not under scale or rotation of parent just translation (position)
	SpaceEngine::setTranslationOnly(parentTransfo);

	// Camera Transformation : parent(Translation) * NodeCamera(Transform) * Cam(Transform)
	glm::mat4 ourworldTransform = parentTransfo * this->getNode()->getMatNodeTransform();

	return ourworldTransform * glm::vec4(m_position, 1.0f);
	
}*/

void ThirdPersonCamera::Update(float deltatime)
{
	
	// Get total transformation from parent
	glm::mat4 transfo = m_targetobj->getTransformationAllIn();
	// Remove Rotation and Scale from parent : 
	// Camera is not under scale or rotation of parent just translation (position)
	SpaceEngine::setTranslationOnly(transfo);

	// Basic World Position
	glm::vec3 target = transfo * glm::vec4(m_targetobj->Position(), 1.0f); 

	// Very basic follow perso at the center of the screen 
	setTargetPoint(target, deltatime);

}

