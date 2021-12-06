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
	void link(); 
	// Use the create shader 
	void use(); 
	// Detach shader 
	void unuse(); 
	// check validity
	bool checkValidity() const; 

	// Uniform
	int getUniformLocation(const char* uniformname); 
	void setUniform4f(const char* uniformname, float x, float y, float z, float w); 
	void setUniform4f(int location, float x, float y, float z, float w);

};

