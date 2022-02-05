#include "Material.h"


Material::Material(Texture* texture, glm::vec3 color)
	: m_texture(texture), m_color(color),
	m_ambient(0.5f), m_diffuse(0.5f), 
	m_specular(0.1f), m_shininess(16.0f)
{};

Material::Material()
	: m_texture(nullptr), m_color(glm::vec3(1.0, 1.0, 1.0)),
	m_ambient(glm::vec3(0.5f)), 
	m_diffuse(glm::vec3(0.5f)),
	m_specular(glm::vec3(0.1f)), 
	m_shininess(16.0f)
{};


Material::Material(const glm::vec3& color,
	const glm::vec3& ambient,
	const glm::vec3& diffuse,
	const glm::vec3& specular,
	const float& shininess)
	: m_ambient(ambient),
	m_diffuse(diffuse),
	m_specular(specular),
	m_shininess(shininess), 
	m_color(color), 
	m_texture(nullptr)
{

}; 

void Material::setTexture(const char* filename) {
	if (m_texture != nullptr) { delete m_texture; }
	m_texture = new Texture(filename);
}

void Material::setTexture(Texture* textu) {
	if (m_texture != nullptr) { 
		delete m_texture; 
	}
	m_texture = textu; 
}

void Material::setShininess(const float& shiny)
{
	m_shininess = shiny > 0.0f ? shiny : 0.0f; 
}
