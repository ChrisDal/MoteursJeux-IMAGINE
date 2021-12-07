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

void ShaderProgram::unuse()
{
	glUseProgram(0); 
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

int ShaderProgram::getUniformLocation(const char* uniformname)
{
	int location = glGetUniformLocation(m_shader, uniformname);
	if (location == -1)
	{
		std::cout << "WARNING::ShaderProgram::UniformLocation : \""
			<< uniformname << "\" not use or not found." << std::endl;
	}

	return location; 
}

void ShaderProgram::setUniform4f(const char* uniformname, float x, float y, float z, float w)
{
	int loc = getUniformLocation(uniformname); 
	glUniform4f(loc, x, y, z, w); 
}

void ShaderProgram::setUniform4f(int location, float x, float y, float z, float w)
{
	glUniform4f(location, x, y, z, w); 
}

void ShaderProgram::setUniform1i(const char* name, int location)
{
	glUniform1i(this->getUniformLocation(name), location);
}
