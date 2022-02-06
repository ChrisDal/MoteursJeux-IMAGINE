#pragma once

#include <vector>
#include <memory>

#include <glm/vec3.hpp>
#include "Texture.h"


class Material
{
private:
	Texture* m_texture; 
	glm::vec3 m_color; 


public: 
	// basic data 
	glm::vec3 m_ambient; 
	glm::vec3 m_diffuse; 
	glm::vec3 m_specular; 
	float m_shininess; 

public: 

	Material(Texture* texture, glm::vec3 color);
	Material();
	Material(const glm::vec3& color, 
		const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular,
		const float& shininess); 

	~Material(); 

	// Getters and Setters 
	void setTexture(const char* filename); 
	void setTexture(Texture* textu); 
	Texture* getTexture()  const { return m_texture; }
	inline bool hasTexture() const {return m_texture != nullptr; }

	void setColor(const glm::vec3& color) { m_color = color; }
	glm::vec3 getColor() const { return m_color; }

	void setAmbient(const float& r, const float& b, const float& g)  { m_ambient = glm::vec3(r, g, b); }
	void setDiffuse(const float& r, const float& b, const float& g)  { m_diffuse = glm::vec3(r, g, b); }
	void setSpecular(const float& r, const float& b, const float& g) { m_specular= glm::vec3(r, g, b); }

	void setShininess(const float& shiny);

public: 
	// Material 
	static std::vector<Material> m_defaults;
	static void initDefaultMaterials(); 
	

};

