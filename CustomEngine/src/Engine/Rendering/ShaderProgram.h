#pragma once
#include "Shader.h"


class ShaderProgram
{
private: 
	unsigned int m_shader; 

public: 

	ShaderProgram(); 
	~ShaderProgram(); 
	// get the program
	unsigned int getShader() const; 
	// Bind  vertex or a fragment shaders
	void bindShader(Shader* shader); 
	void bindShaders(Shader* vertexshader, Shader* fragmentshader); 
	// Link all the attachs shader into one program
	void link() const; 
	// Use the create shader 
	void use() const ; 
	// Detach shader 
	void unuse() const; 
	// check validity
	bool checkValidity() const; 

	// Uniform
	int getUniformLocation(const char* uniformname) const; 

	// 4-Float 
	void setUniform4f(const char* uniformname, float x, float y, float z, float w) const; 
	void setUniform4f(int location, float x, float y, float z, float w) const;
	// glm::vec4
	template <typename T> 
	void setUniform4f(const char* uniformname, const T& vec) const {
		int loc = getUniformLocation(uniformname);
		glUniform4f(loc, vec.x, vec.y, vec.z, vec.w); 
	}

	
	// 3-Float 
	void setUniform3f(const char* uniformname, float x, float y, float z) const;
	void setUniform3f(int location, float x, float y, float z) const; 
	// glm::vec3
	template <typename T>
	void setUniform3f(const char* uniformname, const T& vec) const {
		int loc = getUniformLocation(uniformname);
		glUniform3f(loc, vec.x, vec.y, vec.z);
	}
	// Int 
	void setUniform1i(const char* name, int location) const; 
	// Matrix 4x4 / Float[16] : column major 
	void setMat4(const char* mat4name, const GLfloat* data) const; 

};

