#pragma once

#include <glm/vec3.hpp> 
#include <string>

class GameObject; 

struct Component
{
	int m_id = -1;
	void setId(int number) { m_id = number; }
	int getId() const { return m_id;  }

	std::string m_tag;
	std::string getTag() const { return m_tag;  }
};

struct Light : Component
{
	glm::vec3 color;
	float intensity;

	void setIntensity(float intens) {
		if (intens < 0) { 
			intens = 0.0f;  
		}
		else
		{
			intensity = intens; 
		}
	}

};

// PlayerComponent 
struct Player : Component
{
	std::string m_id = "Player"; 
};


struct Velocity : Component
{
	double vx;
	double vy;
	double vz;

	void setVelocity(double x, double y, double z) {
		vx = x; vy = y; vz = z; 
	}

	void move(const glm::vec3& direction, GameObject* gmo); 


};
