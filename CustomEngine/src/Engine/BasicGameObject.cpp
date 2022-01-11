#include "BasicGameObject.h"
#include "SceneNode.h"


// static define
int BasicGameObject::g_id = 0;

BasicGameObject::BasicGameObject(SceneNode* parent, glm::vec3 center, std::string tag)
	: m_tag(tag), m_internal(SpaceEngine::Transform()), 
	m_position(center), m_parent(parent), m_gparent(nullptr)
{
	parent->addObject(this);
	m_parent = parent;
	// matrice de transformation interne
	m_internal.setAsIdentity();
	m_internal.setTranslate(m_position);
	// matrice de transformation monde
	m_world = glm::translate(glm::mat4x4(1.0f), parent->getPosition());
	m_id = ++g_id;
}

BasicGameObject::BasicGameObject(BasicGameObject* parent, glm::vec3 center, std::string tag)
	: m_tag(tag), m_internal(SpaceEngine::Transform()),
	m_position(center), m_gparent(parent), m_parent(nullptr)
{
	parent->addChild(this); 
	// matrice de transformation interne
	m_internal.setAsIdentity();
	m_internal.setTranslate(m_position);
	// matrice de transformation monde
	m_world = glm::translate(glm::mat4x4(1.0f), parent->Position());
	m_id = ++g_id;

}

BasicGameObject::BasicGameObject()
	: m_tag("Default"), m_internal(SpaceEngine::Transform()), m_parent(nullptr),
	m_position(glm::vec3(0.0f, 0.0f, 0.0f)), m_world(glm::mat4(1.0f)), m_gparent(nullptr)
{
}

BasicGameObject::~BasicGameObject()
{
	std::cout << "Delete BasicGameObj " << getId() << "\n";
}

// ---------------------------------------
// Define Transformations
// ---------------------------------------

void BasicGameObject::setTransformation(SpaceEngine::Transform transfo, bool internal)
{
	if (internal)
	{
		m_internal = transfo;
	}
	else
	{
		m_transfo = transfo;
	}

}

void BasicGameObject::addTransformation(const SpaceEngine::Transform& transfo, bool internal)
{
	if (internal)
	{
		m_internal = m_internal.tfm_combine_with(transfo);
	}
	else
	{
		m_transfo = m_transfo.tfm_combine_with(transfo);
	}

}

SpaceEngine::Transform BasicGameObject::getTransformation(bool internal) const
{
	if (internal)
	{
		return m_internal;
	}

	return m_transfo;
}

void BasicGameObject::applyTransformation()
{
	m_transfo.applyOnPoint(m_position, m_position);
}

glm::mat4x4 BasicGameObject::getMatTransformation()
{
	return glm::mat4x4(1.0f);
}

glm::mat4x4 BasicGameObject::getWorldMat()
{
	if (m_parent != nullptr) {

		return m_parent->getMatWorldTransform();
	}
	else if (m_gparent != nullptr) {

		return m_gparent->getTransformationAllIn()* m_world;
	}
	
}

glm::vec4 BasicGameObject::getWorldPosition()
{
	/*glm::vec3 pos = this->Position();
	SpaceEngine::Transform t_interne = this->getTransformation(true);
	SpaceEngine::Transform t_externe = this->getTransformation();
	glm::mat4x4 worldmat = this->getWorldMat();

	pos = t_interne.applyToPoint(pos, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::vec4 pos4act = worldmat * glm::vec4(pos[0], pos[1], pos[2], 1.0f);
	pos4act = this->getTransformation().getMatrixTransform() * pos4act;*/

	glm::vec4 pos = glm::vec4(this->Position(), 1.0f);

	return this->getTransformationAllIn() * pos; ;
}

glm::mat4x4 BasicGameObject::getTransformationAllIn()
{
	glm::mat4x4 t_interne = this->getTransformation(true).getMatrixTransform();
	glm::mat4x4 t_externe = this->getTransformation().getMatrixTransform();
	glm::mat4x4 worldmat  = this->getWorldMat();

	return t_externe * worldmat * t_interne;
}

void BasicGameObject::addChild(BasicGameObject* obj)
{
	m_children.push_back(obj); 
}


// ==================================================
// Transformations interface at GameObject Level
// ==================================================

// Rotation angle a , angle b , angle g
// True: transfo apply on object space
// False: transfo apply in local space (node)

BasicGameObject& BasicGameObject::Rotate(float alpha, float beta, float gamma, bool internal)
{
	if (internal)
	{
		m_internal.addRotation(alpha, beta, gamma);
	}
	else
	{
		m_transfo.addRotation(alpha, beta, gamma);
	}
	return *this;
}

// Translation x axis, y axis, y axis
// True: transfo apply on object space
// False: transfo apply in local space (node)
BasicGameObject& BasicGameObject::Translate(float tx, float ty, float tz, bool internal)
{
	if (internal)
	{
		m_internal.addTranslate(tx, ty, tz);
	}
	else
	{
		m_transfo.addTranslate(tx, ty, tz);
	}
	return *this;
}

// Scaling on x axis, y axis, y axis
// True: transfo apply on object space
// False: transfo apply in local space (node)
BasicGameObject& BasicGameObject::Scale(float sx, float sy, float sz, bool internal)
{
	if (internal)
	{
		m_internal.addScale(sx, sy, sz);
	}
	else
	{
		m_transfo.addScale(sx, sy, sz);
	}

	return *this;
}

glm::vec3 BasicGameObject::Position() const
{
	return m_position; 
}

void BasicGameObject::Position(float x, float y, float z)
{
	m_position = glm::vec3(x, y, z);
	//m_world = glm::translate(glm::mat4x4(1.0f), m_position);
	m_internal.setTranslate(m_position[0], m_position[1], m_position[2]);
}


void BasicGameObject::Position(const glm::vec3& pos)
{
	m_position = pos;
	m_internal.setTranslate(m_position[0], m_position[1], m_position[2]); 
}