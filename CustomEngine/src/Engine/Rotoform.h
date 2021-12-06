#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <ostream>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat3x3.hpp> // glm::mat4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

// Class of Rotation for transformation matrix 
// angle are given by the user in degrees 
// Store in class in radians 
class Rotoform
{
private:

	glm::mat3x3 m_rotx; // rotation axe x angle alpha 
	glm::mat3x3 m_roty; // rotation axe y 
	glm::mat3x3 m_rotz; // rotation axe z 
	glm::mat3x3 m_rotation; // global rotation matrix 

protected:

	// process the global transformation of the class
	// Rotation = roty*rotx*rotz 
	inline void processTransformation();

	// convert degrees to rad 
	inline float degreeToRad(const float& theta);

public:

	// Get a matrix rotation from an angle in degrees and an axis 
	inline glm::mat3x3 getMat3rotationX(float angle);
	inline glm::mat3x3 getMat3rotationY(float angle);
	inline glm::mat3x3 getMat3rotationZ(float angle);

	// Get a matrix rotation from an angle in radians and an axis 
	inline glm::mat3x3 getMat3rotationX_rad(float eta);
	inline glm::mat3x3 getMat3rotationY_rad(float eta);
	inline glm::mat3x3 getMat3rotationZ_rad(float eta);

	// Set Rotation on axis according to theta in degrees  
	inline void setRotationX(float theta);
	inline void setRotationY(float theta);
	inline void setRotationZ(float theta);

	// Set Rotation on axis according to eta in radians
	inline void setRotationX_rad(float eta);
	inline void setRotationY_rad(float eta);
	inline void setRotationZ_rad(float eta);


public:

	// Constructeurs 
	Rotoform();
	Rotoform(const glm::mat3x3& rotx, const glm::mat3x3& roty,
		const glm::mat3x3 & rotz);
	Rotoform(float alpha, float beta, float gamma, bool inRad);

	// Copy Constructor
	Rotoform(const Rotoform& source);
	// Copy Assignement 
	Rotoform& operator=(const Rotoform& source);

	// Move Constructor
	Rotoform(Rotoform&& source);
	// Destructor 
	~Rotoform();



	// Set Global rotation = set value of rotation
	void setRotation(float alpha, float beta, float gamma);
	void setRotation_rad(float alpha, float beta, float gamma);


	// Add Rotation on axis according to theta in degrees  
	void addRotationX(float theta);
	void addRotationY(float theta);
	void addRotationZ(float theta);

	// Add Rotation on axis according to eta in radians  
	void addRotationX_rad(float eta);
	void addRotationY_rad(float eta);
	void addRotationZ_rad(float eta);

	// Add Global rotation = rotate previous matrix 
	void addRotation(float alpha, float beta, float gamma);
	void addRotation_rad(float alpha, float beta, float gamma);

	// Reset global
	void resetRotation();


	// Getters 
	inline glm::mat3x3 getRotX() const { return m_rotx; }
	inline glm::mat3x3 getRotY() const { return m_roty; }
	inline glm::mat3x3 getRotZ() const { return m_rotz; }

	// Get global matrix rotation
	glm::mat3x3 getRotation() const;
	inline void setMatrixRotation(const glm::mat3x3& mat);

	// Get inverse rotation => transpose 
	inline glm::mat3x3 getInvMatRot();
	Rotoform getInvRot();

	// Apply
	//glm::vec3 applyToPoint(const glm::vec3& point);
	glm::vec3 applyToPoint(const glm::vec3& point, const glm::vec3& rotcenter);
	glm::vec3 applyToVector(const glm::vec3& vec);
	glm::vec3 applyToVersor(const glm::vec3& versor);

	// Operation on Rotoform directly 
	Rotoform operator+(const Rotoform& other);
	Rotoform operator-(const Rotoform& other) = delete;
	Rotoform operator*(const Rotoform& other) = delete;
	Rotoform operator/(const Rotoform& other) = delete;

	friend std::ostream& operator<<(std::ostream& stream, const Rotoform& rotation);

};

