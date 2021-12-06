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
	void unlink(); 
	// check validity
	bool checkValidity() const; 

	int getUniformLocation(const char* uniformname); 

};

