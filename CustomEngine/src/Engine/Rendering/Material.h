#pragma once

#include "Texture.h"
#include <glm/vec3.hpp>

class Material
{
private:
	Texture* m_texture; 
	glm::vec3 m_color; 

public: 

	Material(Texture* texture, glm::vec3 color)
		: m_texture(texture), m_color(color)
	{}; 

	Material()
		: m_texture(nullptr), m_color(glm::vec3(1.0, 1.0, 1.0))
	{};

	void setTexture(const char* filename) { 
		if (m_texture != nullptr) { delete m_texture;  }
		m_texture = new Texture(filename);  
	}
	void setTexture(Texture* textu) {m_texture = textu; }
	void setColor(const glm::vec3&  color) {m_color = color; }
	glm::vec3 getColor() const { return m_color;  }
	Texture* getTexture()  const { return m_texture; }
};

