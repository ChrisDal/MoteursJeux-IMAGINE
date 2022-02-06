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

	// obsidian 
	m_defaults.emplace_back(
		defaultcolor,
		glm::vec3(0.05375,0.05,0.06625),
		glm::vec3(0.18275, 0.17,0.22525),
		glm::vec3(0.332741,0.328634, 0.346435),
		0.3
	);

	m_defaultnames.push_back("Obsidian");

	// pearl
	m_defaults.emplace_back(
		defaultcolor,
		glm::vec3(0.25f, 0.20725f, 0.20725f),
		glm::vec3(1.0f, 0.829f, 0.829f),
		glm::vec3(0.296648, 0.296648, 0.296648),
		0.088f
	);

	m_defaultnames.push_back("Pearl");

	// ruby
	m_defaults.emplace_back(
		defaultcolor,
		glm::vec3(0.1745f, 0.01175f, 0.01175f),
		glm::vec3(0.61424f, 0.04136f, 0.04136f),
		glm::vec3(0.727811f, 0.626959f, 0.626959f),
		0.6f
	);

	m_defaultnames.push_back("Ruby");

	// turquoisef
	m_defaults.emplace_back(
		defaultcolor,
		glm::vec3(0.1f, 0.18725f, 0.1745f),
		glm::vec3(0.396f, 0.74151f, 0.69102f),
		glm::vec3(0.297254f, 0.30829f, 0.306678f),
		0.1f
	);

	m_defaultnames.push_back("Turquoise");

	// Brass
	m_defaults.emplace_back(
		defaultcolor,
		glm::vec3(0.329412f, 0.223529f, 0.027451f),
		glm::vec3(0.780392f, 0.568627f, 0.113725f),
		glm::vec3(0.992157f, 0.941176f, 0.807843f),
		0.21794872f
	);

	m_defaultnames.push_back("Brass");

	// bronze
	m_defaults.emplace_back(
		defaultcolor,
		glm::vec3(0.2125f, 0.1275f, 0.054f),
		glm::vec3(0.714f, 0.4284f, 0.18144f),
		glm::vec3(0.393548f, 0.271906f, 0.166721f),
		0.2f
	);

	m_defaultnames.push_back("Bronze");


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
