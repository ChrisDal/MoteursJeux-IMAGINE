#include "Material.h"


std::vector<Material> Material::m_defaults = {};
std::vector<std::string> Material::m_defaultnames = {}; 


void Material::initDefaultMaterials()
{
	m_defaults.clear();
	glm::vec3 defaultcolor = glm::vec3(1.0f);

	// Emerald 
	m_defaults.emplace_back(
		defaultcolor,
		glm::vec3(0.0215, 0.1745, 0.0215),
		glm::vec3(0.07568, 0.61424, 0.07568),
		glm::vec3(0.633, 0.727811, 0.633),
		0.6f
	);

	m_defaultnames.push_back("Emerald"); 

	// jade 
	m_defaults.emplace_back(
		defaultcolor,
		glm::vec3(0.135, 0.2225, 0.1575),
		glm::vec3(0.54, 0.89, 0.63),
		glm::vec3(0.316228),
		0.1f
	);

	m_defaultnames.push_back("Jade");

}

Material::Material(Texture* texture, glm::vec3 color)
	: m_texture(texture), m_color(color),
	m_ambient(0.5f), m_diffuse(0.5f), 
	m_specular(0.1f), m_shininess(0.125f)
{};

Material::Material()
	: m_texture(nullptr), m_color(glm::vec3(0.8f, 1.0f, 1.0f)),
	m_ambient(glm::vec3(0.25f)),  // Chrome
	m_diffuse(glm::vec3(0.4f)),
	m_specular(glm::vec3(0.774597f)),
	m_shininess(0.6f)
{};


Material::Material(const glm::vec3& color,
	const glm::vec3& ambient,
	const glm::vec3& diffuse,
	const glm::vec3& specular,
	const float& shininess)
	: m_ambient(ambient),
	m_diffuse(diffuse),
	m_specular(specular),
	m_color(color), 
	m_texture(nullptr)
{
	setShininess(shininess); 
}
Material::~Material()
{};

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
	m_shininess = shiny > 0.0f ? shiny : 1e-4f; ;
}
