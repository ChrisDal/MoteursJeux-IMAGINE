#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class Shader
{

private:
	const char* m_source;
	std::string m_ssource; 
	unsigned int m_shader;
	GLenum m_type;  // Vertex or Fragment
	bool m_valid;

public:

	Shader(); 
	Shader(const char* filepath, GLenum shaderType);
	~Shader(); 
	
	bool loadSourceShader(const char* filepath);

	unsigned int getShader() const;
	bool checkValidity(); 

	
};

