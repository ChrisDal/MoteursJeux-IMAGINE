#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
	: m_shader(0)
{
	m_shader = glCreateProgram(); 
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_shader); 
}

unsigned int ShaderProgram::getShader() const
{
	return m_shader;
}

void ShaderProgram::bindShader(Shader* shader)
{
	glAttachShader(m_shader, shader->getShader()); 
}

void ShaderProgram::bindShaders(Shader* vertexshader, Shader* fragmentshader)
{
	glAttachShader(m_shader, vertexshader->getShader());
	glAttachShader(m_shader, fragmentshader->getShader());
}

void ShaderProgram::link()
{
	glLinkProgram(m_shader); 
}

void ShaderProgram::use()
{
	glUseProgram(m_shader); 
}

bool ShaderProgram::checkValidity() const
{
	int success; 
	char infoLog[512];
	glGetProgramiv(m_shader, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(m_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::ATTACH::LINK_FAILED\n" << infoLog << std::endl; 
	}

	return false;
}

