#include "Transform.h"
#include <iostream>


namespace SpaceEngine {

	void Transform::setAsIdentity()
	{
		m_transform = glm::mat4(1.0f);
	}

	// --------------------------------------------
	// Reset Transformations 
	// --------------------------------------------

	// set translation to QVector3(0)
	void Transform::resetTranslation()
	{
		m_translation.x = 0.0f;
		m_translation.y = 0.0f;
		m_translation.z = 0.0f;
	}

	// set Scale to 1.0f
	void Transform::resetScale()
	{
		m_scale.x = 1.0f;
		m_scale.y = 1.0f;
		m_scale.z = 1.0f;
	}

	// set rotation to identity
	void Transform::resetRotation()
	{
		m_rotation.resetRotation();
	}


	// set transformation matrix to identity
	void Transform::resetMat4x4Transformation()
	{
		m_transform = glm::mat4(1.0f);
	}


	// reset all transformations 
	void Transform::reset()
	{
		resetScale();
		resetRotation();
		resetTranslation();
		resetMat4x4Transformation();
	};


	// ================================================
	// Constructors 
	// ================================================

	Transform::Transform()
		: m_scale(glm::vec3(1.0f, 1.0f, 1.0f)), m_translation(glm::vec3(0.0f, 0.0f, 0.0f))
	{
		m_rotation = Rotoform();

		// global transform 
		m_transform = glm::mat4(1.0f);

	}


	Transform::Transform(glm::vec3 scaleVec3, Rotoform rotMat, glm::vec3 transVec3)
		: m_scale(scaleVec3), m_rotation(rotMat), m_translation(transVec3)
	{
		// init global transform 
		m_transform = glm::mat4(1.0f);

		processMat4x4Transform();
	}

	// Copy constructor 
	Transform::Transform(const Transform& source)
	{
		m_scale = source.m_scale;
		m_rotation = source.m_rotation;
		m_translation = source.m_translation;

		m_transform = source.m_transform;

	}

	// Copy assignement operator  
	Transform& Transform::operator=(const Transform& source)
	{

		if (this == &source)
			return *this;

		m_scale = source.m_scale;
		m_rotation = source.m_rotation;
		m_translation = source.m_translation;

		m_transform = source.m_transform;

		return *this;
	}

	// Move constructor 
	Transform::Transform(Transform&& source)
	{
		m_scale = source.m_scale;
		m_rotation = source.m_rotation;
		m_translation = source.m_translation;

		m_transform = source.m_transform;

	}

	// Destructor
	Transform::~Transform() {};


	// unable to retrieve composantes 
	Transform::Transform(const glm::mat4& rmat4)
		: m_transform(rmat4)
	{
		std::cout << rmat4[0][0];
		m_translation = glm::vec3(rmat4[3]);
		m_rotation = Rotoform();
		m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	// process 4x4 Matrix Transform
	inline void Transform::processMat4x4Transform()
	{

		// 1. scaling, 2.rotation, 
		/*glm::mat3x3 scaled = glm::mat3x3();
		scaled = glm::mat3x3(1.0f);
		scaled[0][0] *= m_scale.x;
		scaled[1][1] *= m_scale.y;
		scaled[2][2] *= m_scale.z;*/
		// Rotation
		//glm::mat3x3 rotscaled = scaled * m_rotation.getRotation();

		// 1. Rotation
		glm::mat3x3 rotscaled = m_rotation.getRotation();
		// 2. Scale
		rotscaled[0][0] *= m_scale.x;
		rotscaled[1][1] *= m_scale.y;
		rotscaled[2][2] *= m_scale.z;


		

		// Tranform matrix
		m_transform = glm::mat4(1.0f);
		m_transform[0][0] = rotscaled[0][0];
		m_transform[0][1] = rotscaled[0][1];
		m_transform[0][2] = rotscaled[0][2];
		m_transform[1][0] = rotscaled[1][0];
		m_transform[1][1] = rotscaled[1][1];
		m_transform[1][2] = rotscaled[1][2];
		m_transform[2][0] = rotscaled[2][0];
		m_transform[2][1] = rotscaled[2][1];
		m_transform[2][2] = rotscaled[2][2];

		// 3. apply translation
		m_transform[3][0] = m_translation.x;
		m_transform[3][1] = m_translation.y;
		m_transform[3][2] = m_translation.z;
		
	}



	// ================================================
	// ROTATION 
	// ================================================

	// Set transformation as rotation in degrees around an axis
	void Transform::setRotationX(float angle) { m_rotation.setRotationX(angle); }
	void Transform::setRotationY(float angle) { m_rotation.setRotationY(angle); }
	void Transform::setRotationZ(float angle) { m_rotation.setRotationZ(angle); }

	// Set transformation as rotation in radians around an axis
	void Transform::setRotationX_rad(float angle) { m_rotation.setRotationX_rad(angle); }
	void Transform::setRotationY_rad(float angle) { m_rotation.setRotationY_rad(angle); }
	void Transform::setRotationZ_rad(float angle) { m_rotation.setRotationZ_rad(angle); }


	// Add rotation(angle) in degrees on axis given to current matrix rotation
	void Transform::addRotation(float alpha, float beta, float gamma) { m_rotation.addRotation(alpha, beta, gamma); }

	// Add rotation(angle) in radians on axis given to current matrix rotation
	void Transform::addRotation_rad(float alpha, float beta, float gamma) { m_rotation.addRotation_rad(alpha, beta, gamma); }


	// Set rotation(anglex, angley, anglez)
	void Transform::setRotation(float alpha, float beta, float gamma) { m_rotation.setRotation(alpha, beta, gamma); }

	// Set rotation(anglex, angley, anglez)
	void Transform::setRotation_rad(float alpha, float beta, float gamma) { m_rotation.setRotation_rad(alpha, beta, gamma); }

	// Return Rotation Matrix 3x3 
	inline Rotoform Transform::getRotation() const { return m_rotation; }
	glm::mat3x3 Transform::getRotationMat() const { return this->m_rotation.getRotation(); }



	// Personal Transformation : Rotate  
	Transform& Transform::Rotate(float alpha, float beta, float gamma)
	{
		this->addRotation(alpha, beta, gamma);
		return *this;
	}


	// ================================================
	// SCALING 
	// ================================================

	// set an homogenous scaling 
	void Transform::setHomogenousScale(float scale_factor)
	{
		m_scale.x = scale_factor;
		m_scale.y = scale_factor;
		m_scale.z = scale_factor;
	}


	// set an inhomogenous scaling 
	void Transform::setScale(float sx, float sy, float sz)
	{
		m_scale.x = sx;
		m_scale.y = sy;
		m_scale.z = sz;

	}

	// rescale : add an homogenous scaling = m_scale*s 
	void Transform::addHomogenousScale(float scale_factor)
	{
		m_scale *= scale_factor;
	}

	// add an inhomogenous scaling 
	void Transform::addScale(float sx, float sy, float sz)
	{
		m_scale.x *= sx;
		m_scale.y *= sy;
		m_scale.z *= sz;
	}


	// set the scaling vector
	void Transform::setScale(const glm::vec3& matscale)
	{
		m_scale = matscale;
	}


	// Personal Transformation : Scale  
	Transform& Transform::Scale(float sx, float sy, float sz)
	{
		this->addScale(sx, sy, sz);
		return *this;
	}


	// ================================================
	// TRANSLATION 
	// ================================================

	// set an homogenous translate 
	void Transform::setHomogenousTranslate(float txyz)
	{
		m_translation.x = txyz;
		m_translation.y = txyz;
		m_translation.z = txyz;
	}

	// add a translation : tx, ty, tz
	void Transform::addTranslate(float tx = 0.0f, float ty = 0.0f, float tz = 0.0f)
	{
		m_translation.x += tx;
		m_translation.y += ty;
		m_translation.z += tz;
	}

	// add a translation : vec3 
	void Transform::addTranslate(const glm::vec3& txyz)
	{
		m_translation.x += txyz.x;
		m_translation.y += txyz.y;
		m_translation.z += txyz.z;
	}

	// add a translation += (txyz, txyz, txyz)
	void Transform::addHomogenousTranslate(float txyz)
	{
		m_translation.x += txyz;
		m_translation.y += txyz;
		m_translation.z += txyz;
	}



	void Transform::setTranslate(const glm::vec3& translation)
	{
		m_translation = translation;
	}


	void Transform::setTranslate(float tx, float ty, float tz)
	{
		m_translation.x = tx;
		m_translation.y = ty;
		m_translation.z = tz;
	}


	// Personal Transformation : Translate  
	Transform& Transform::Translate(float tx, float ty, float tz)
	{
		this->addTranslate(tx, ty, tz);
		return *this;
	}


	// ================================================
	// OTHER 
	// ================================================

	void Transform::applyToMatrix(glm::mat4& MatIn)
	{
		MatIn = m_transform * MatIn;
	}

	// Matrix multiplication4x4
	glm::mat4 Transform::Multiply(const glm::mat4& MatIn)
	{
		return m_transform * MatIn;
	}


	// Matrix multiplication4x4
	glm::mat4 Transform::combine_with(Transform MatIn)
	{
		glm::mat4 combined = this->getMatrixTransform() * MatIn.getMatrixTransform();

		return combined;
	}

	Transform Transform::tfm_combine_with(Transform MatIn)
	{
		Transform combined = Transform(*this);
		combined.m_rotation = combined.m_rotation + MatIn.m_rotation;
		combined.m_translation += MatIn.m_translation;
		combined.m_scale *= MatIn.m_scale;

		return combined;
	}


	// ================================================
	// APPLY TO 
	// ================================================

	// Apply Transformation to Point : Scale + Rotation + Translation
	void Transform::applyOnPoint(glm::vec3& position, const glm::vec3& center)
	{
		// apply scale first
		position[0] *= m_scale[0];
		position[1] *= m_scale[1];
		position[2] *= m_scale[2];

		// apply rotation
		position = m_rotation.applyToPoint(position, center);

		// apply translation
		position += m_translation;

	}

	// Apply Transformation to Vector : Scale + Rotation
	void Transform::applyOnVector(glm::vec3& vec3)
	{
		// apply scale first 
		vec3[0] *= m_scale[0];
		vec3[1] *= m_scale[1];
		vec3[2] *= m_scale[2];

		// apply rotation 
		vec3 = m_rotation.applyToVector(vec3);

	}


	// Apply Transformation to Versor : Rotation
	void Transform::applyOnVersor(glm::vec3& versor)
	{
		// apply rotation 
		versor = m_rotation.applyToVersor(versor);

	}




	// Apply Transformation to Vec3 Point with a center
	glm::vec3 Transform::applyToPoint(const glm::vec3& position, const glm::vec3& center)
	{
		glm::vec3 presult = glm::vec3(position);
		// apply scale first
		presult[0] *= m_scale[0];
		presult[1] *= m_scale[1];
		presult[2] *= m_scale[2];

		// apply rotation
		presult = m_rotation.applyToPoint(position, center);

		// apply translation
		presult += m_translation;

		return presult;
	}

	// Apply Transformation to Vec3
	glm::vec3 Transform::applyToVector(const glm::vec3& vec3)
	{
		glm::vec3 presult = glm::vec3(vec3);
		// apply scale first 
		presult[0] *= m_scale[0];
		presult[1] *= m_scale[1];
		presult[2] *= m_scale[2];

		// apply rotation 
		presult = m_rotation.applyToVector(vec3);

		return presult;
	}

	// Apply Transformation to Versor
	glm::vec3 Transform::applyToVersor(const glm::vec3& versor)
	{
		glm::vec3 presult = glm::vec3(versor);
		// apply rotation 
		presult = m_rotation.applyToVersor(versor);

		return presult;
	}


	// ================================================
	// Getters Transformation Matrix
	// ================================================

	Transform Transform::getTransform() const {
		return *this;
	}


	glm::mat4 Transform::getMatrixTransform() {
		this->processMat4x4Transform();
		return m_transform;
	}
	glm::mat4 Transform::getTransposedTransform() {
		return this->getInverseTransform().getMatrixTransform();
	}

	// not implemented : return Id
	Transform Transform::getInverseTransform()
	{
		Transform invtransfo;
		invtransfo.m_rotation = this->m_rotation.getInvRot();
		invtransfo.m_translation = -this->m_translation;
		invtransfo.m_scale = glm::vec3(1.0f, 1.0f, 1.0f) / this->m_scale;

		return invtransfo;
	}

};

std::ostream& operator<<(std::ostream& stream, const SpaceEngine::Transform& transfo)
{
	stream << "Translation : " << transfo.getTranslation().x << "," << transfo.getTranslation().y << "," << transfo.getTranslation().z << std::endl;
	stream << "Scaling : " << transfo.getScale().x << "," << transfo.getScale().y << "," << transfo.getScale().z << std::endl;
	stream << transfo.getRotation() << std::endl;

	return stream; 
}




