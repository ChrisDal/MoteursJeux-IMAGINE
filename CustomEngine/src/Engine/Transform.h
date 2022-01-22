#pragma once
#include <glad/glad.h>
#include <ostream>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat3x3.hpp> // glm::mat3
#include <glm/mat4x4.hpp> // glm::mat4

#include <glm/gtc/type_ptr.hpp>
#include "Rotoform.h"


namespace SpaceEngine{

class Transform
{

private:
	// Decomposition : scaling, rotation, translation
	glm::vec3 m_scale;
	Rotoform m_rotation;
	glm::vec3 m_translation;

	glm::mat4x4 m_transform; // Full matrix transformation

protected:
	inline void processMat4x4Transform();
	Transform(const glm::mat4x4& mat4);

	void resetTranslation();
	void resetScale();
	void resetRotation();
	void resetMat4x4Transformation();

	// Rotoform 

public:

	// --------------------------------------------
	// Constructors
	// --------------------------------------------
	Transform();
	Transform(glm::vec3 scaleVec3, Rotoform rotMat, glm::vec3 transVec3);
	Transform(const Transform& source);
	Transform& operator=(const Transform& source);
	Transform(Transform&& source) noexcept;

	~Transform(); // destructor 



	// --------------------------------------------
	// Reset Transformations
	// --------------------------------------------

	void reset();
	void setAsIdentity();

	// --------------------------------------------
	// ROTATION 
	// --------------------------------------------

	// Set transformation as rotation around x axis
	void setRotationX(float angle);

	// Set transformation as rotation around y axis
	void setRotationY(float angle);

	// Set transformation as rotation around z axis
	void setRotationZ(float angle);

	// Set transformation as rotation around x axis
	void setRotationX_rad(float angle);

	// Set transformation as rotation around y axis
	void setRotationY_rad(float angle);

	// Set transformation as rotation around z axis
	void setRotationZ_rad(float angle);

	// Add rotation(angle) on axis given to current matrix rotation
	void addRotation(float alpha, float beta, float gamma);
	// Add rotation(angle) on axis given to current matrix rotation
	void addRotation_rad(float alpha, float beta, float gamma);

	// Set rotation(anglex, angley, anglez)
	void setRotation(float alpha, float beta, float gamma);
	// Set rotation(anglex, angley, anglez)
	void setRotation_rad(float alpha, float beta, float gamma);

	// Personal Transformation : Rotate  
	Transform& Rotate(float alpha, float beta, float gamma);

	glm::mat3x3 getRotationMat() const;
	inline Rotoform getRotation() const;

	void setRotationMat(const glm::mat3x3& mat); 

	// --------------------------------------------
	// SCALING 
	// --------------------------------------------

	// set an homogenous scaling 
	void setHomogenousScale(float scale_factor);

	// set an inhomogenous scaling 
	void setScale(float sx, float sy, float sz);

	// add an homogenous scaling 
	void addHomogenousScale(float scale_factor);

	// add an inhomogenous scaling 
	void addScale(float sx, float sy, float sz);

	// set the scaling matrix
	void setScale(const glm::vec3& matscale);

	// Personal Transformation : Scale  
	Transform& Scale(float sx, float sy, float sz);

	// Get Scale
	glm::vec3 getScale() const { return m_scale; }

	// --------------------------------------------
	// TRANSLATION  
	// --------------------------------------------
	// set an homogenous translate 
	void setHomogenousTranslate(float txyz);

	// add a translation : tx, ty, tz
	void addTranslate(float tx, float ty, float tz);
	void addTranslate(const glm::vec3& txyz);
	// add a translation += (txyz, txyz, txyz)
	void addHomogenousTranslate(float txyz);

	// set translation 
	void setTranslate(const glm::vec3& translation);
	void setTranslate(float tx, float ty, float tz);

	// Personal Transformation : Translate  
	Transform& Translate(float tx, float ty, float tz);

	glm::vec3 getTranslation() const { return m_translation; }

	// --------------------------------------------
	// OPERATIONS  
	// --------------------------------------------
	void applyToMatrix(glm::mat4x4& MatIn);

	// Matrix multiplication4x4
	glm::mat4x4 Multiply(const glm::mat4x4& MatIn);

	// Apply ON
	//void applyOnPoint(glm::vec3& position);
	void applyOnPoint(glm::vec3& position, const glm::vec3& center);
	void applyOnVector(glm::vec3& vec3);
	void applyOnVersor(glm::vec3& versor);


	glm::vec3 applyToPoint(const glm::vec3& position, const glm::vec3& center);
	glm::vec3 applyToVector(const glm::vec3& vec3);
	glm::vec3 applyToVersor(const glm::vec3& versor);

	// --------------------------------------------
	// Transform  
	// --------------------------------------------
	Transform getTransform() const;

	glm::mat4x4 getMatrixTransform();
	glm::mat4x4 getTransposedTransform();
	Transform getInverseTransform();

	// Transformation Combined
	glm::mat4x4 combine_with(Transform MatIn);
	Transform tfm_combine_with(Transform MatIn);

	// use with openGL 
	inline const GLfloat* getTransformGL() const { return glm::value_ptr(m_transform); }

	


};

};

std::ostream& operator<<(std::ostream& stream, const SpaceEngine::Transform& transfo);

