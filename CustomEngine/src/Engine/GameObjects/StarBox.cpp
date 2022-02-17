#include "StarBox.h"

StarBox::StarBox(SceneNode* parent, glm::vec3 center, std::string tag, float radius, int stars)
	: BasicGameObject(parent, center, tag), 
	m_radiusStars(radius), m_nstars(stars)
{
	m_mesh = new Mesh();
}

StarBox::~StarBox()
{
	if (m_mesh != nullptr)
	{
		delete m_mesh;
	}

	std::cout << "Delete StarBox " << getId() << "\n";
}

void StarBox::setbbox()
{
}

void StarBox::setStarsParameters(float radius, int nstars)
{
	m_radiusStars = radius; 
	m_radiusStars = m_nstars; 
}

void StarBox::setMesh(Mesh* mesh) 
{
	if (m_mesh != nullptr) { 
		delete m_mesh; 
	}

	m_mesh = mesh;
}

void StarBox::print()
{
	std::cout << "[SkyStars" << m_id << "] Position " << m_position.x << ',' << m_position.y << "," << m_position.z << "\n";
}

void StarBox::initMesh(const std::string& vertexshader, const std::string& fragshader)
{
	m_mesh->initStarsSky(vertexshader, fragshader,
		             	m_radiusStars, m_nstars);
}

void StarBox::Update(float deltatime)
{
}
