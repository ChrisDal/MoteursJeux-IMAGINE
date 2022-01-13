#pragma once
#include "Mesh.h"
class Sphere : public Mesh
{
private: 
	float m_radius; 
	glm::vec3 m_center; 

	float dphi;
	float drho;

	

protected: 
	void init(float radius, const glm::vec3& center); 

public: 
	Sphere(float radius, const glm::vec3& center); 
	Sphere(); 
	~Sphere() {};

	glm::vec3 getCenter() const { return m_center;  }
	void setdphi(float deltadegree) { dphi = deltadegree;  }
	void setdrho(float deltadegree) { drho = deltadegree;  }
};

