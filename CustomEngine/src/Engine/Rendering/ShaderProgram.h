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
	void setUniform4f(const char* uniformname, float x, float y, float z, float w); 
	void setUniform4f(int location, float x, float y, float z, float w);
	void setUniform1i(const char* name, int location) const; 
	void setMat4(const char* mat4name, const GLfloat* data) const; 

};

