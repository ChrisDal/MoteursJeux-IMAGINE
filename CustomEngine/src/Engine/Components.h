#pragma once

#include <glm/vec3.hpp> 
#include <string>

class BasicGameObject; 

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
	glm::vec3 m_color;
	float intensity;

	void setIntensity(float intens) {
		intensity = intens < 0.0f ? 0.0f : intens; 
	}

	void setLightColor(const glm::vec3& color) {
		m_color = color; 
	}

};

// PlayerComponent 
struct Player : Component
{
	std::string m_id = "Player"; 
};


struct Velocity : Component
{
	double vx = 0.0f;
	double vy = 0.0f;
	double vz = 0.0f;

	// Max and min speed 
	double max_v = 50.0;
	double min_v = -max_v;

	double dv = 0.5; // reduction of speed 
	static double DEAD_SPEED; 

	void setVelocity(double x, double y, double z) {
		vx = x; vy = y; vz = z; 
	}

	void move(BasicGameObject* gmo, float dt); 
	inline void clampV(); 
	inline bool ismoving() const { return vx != 0.0 || vy != 0.0 || vz != 0.0;  }


};

struct Living : Component
{
	bool m_dead = false; 

};
