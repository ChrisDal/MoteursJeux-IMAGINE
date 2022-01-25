#include "BasicGameObject.h"
#include "SceneNode.h"


// static define
int BasicGameObject::g_id = 0;

BasicGameObject::BasicGameObject(SceneNode* parent, glm::vec3 center, std::string tag)
	: m_tag(tag), m_internal(SpaceEngine::Transform()), 
	m_position(center), m_parent(parent)
{
	parent->addObject(this);
	m_parent = parent;
	// matrice de transformation interne
	m_internal.setAsIdentity();
	m_internal.setTranslate(m_position);
	m_id = ++g_id;
}

/*BasicGameObject::BasicGameObject(BasicGameObject* parent, glm::vec3 center, std::string tag)
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

}*/

BasicGameObject::BasicGameObject()
	: m_tag("Default"), m_internal(SpaceEngine::Transform()), m_parent(nullptr),
	m_position(glm::vec3(0.0f, 0.0f, 0.0f))
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
		m_parent->setTransformation(transfo, false); 
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
		m_parent->addTransformation(transfo);
	}

}

// Get Object Transformationq
SpaceEngine::Transform BasicGameObject::getTransformation() const
{
	return m_internal;

}

void BasicGameObject::applyTransformation()
{
	m_internal.applyOnPoint(m_position, m_position);
}

glm::mat4x4 BasicGameObject::getMatTransformation()
{
	return m_internal.getMatrixTransform();
}

glm::mat4x4 BasicGameObject::getWorldMat()
{
	if (m_parent != nullptr) {

		return m_parent->getMatWorldTransform();
	}

	return glm::mat4x4(1.0f); 
	
}

glm::vec4 BasicGameObject::getWorldPosition()
{
	glm::vec4 pos = glm::vec4(m_position, 1.0f);

	return this->getTransformationAllIn() * pos; ;
}

glm::mat4x4 BasicGameObject::getTransformationAllIn()
{
	// Transformation of game object 
	glm::mat4x4 t_interne = this->getTransformation().getMatrixTransform();
	// Scene Node global transformation
	glm::mat4x4 t_externe = m_parent->getMatTransform(); 

	return t_externe * t_interne;
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
		m_parent->Rotate(alpha, beta, gamma, false);
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
		m_parent->Translate(tx, ty, tz, false); 
	}
	

	return *this;
}

BasicGameObject& BasicGameObject::Translate(const glm::vec3& transvec, bool internal)
{
	if (internal)
	{
		m_internal.addTranslate(transvec.x, transvec.y, transvec.z);
	}
	else
	{
		m_parent->Translate(transvec.x, transvec.y, transvec.z, false);
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
		m_parent->Scale(sx, sy, sz, false);
	}

	return *this;
}

BasicGameObject& BasicGameObject::setRotate(float alpha, float beta, float gamma, bool internal)
{
	if (internal)
	{
		m_internal.setRotation(alpha, beta, gamma);
	}
	else
	{
		m_parent->Rotate(alpha, beta, gamma, false);
	}
	return *this;
}

BasicGameObject& BasicGameObject::setRotate(const glm::vec3& rotvec, bool internal)
{
	return setRotate(rotvec.x, rotvec.y, rotvec.z, internal); 
}

BasicGameObject& BasicGameObject::setTranslate(float tx, float ty, float tz, bool internal)
{
	if (internal)
	{
		m_internal.setTranslate(tx, ty, tz);
	}
	else
	{
		m_parent->Translate(tx, ty, tz, false);
	}


	return *this;
}

BasicGameObject& BasicGameObject::setTranslate(const glm::vec3& transvec, bool internal)
{
	return setTranslate(transvec.x, transvec.y, transvec.z, internal);
}

BasicGameObject& BasicGameObject::setScale(float sx, float sy, float sz, bool internal)
{
	if (internal)
	{
		m_internal.setScale(sx, sy, sz);
	}
	else
	{
		m_parent->Scale(sx, sy, sz, false);
	}

	return *this;
}

BasicGameObject& BasicGameObject::setScale(const glm::vec3& scalevec, bool internal)
{
	return setScale(scalevec.x, scalevec.y, scalevec.z, internal); 
}

// Global World Position
glm::vec3 BasicGameObject::Position() const
{
	
	//glm::vec4 pos = m_parent->getMatWorldTransform() * glm::vec4(m_position, 1.0f);
	return  m_position; 
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