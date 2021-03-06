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

void ShaderProgram::link() const
{
	glLinkProgram(m_shader); 
}

void ShaderProgram::use() const
{
	glUseProgram(m_shader); 
}

void ShaderProgram::unuse() const
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

int ShaderProgram::getUniformLocation(const char* uniformname) const
{
	int location = glGetUniformLocation(m_shader, uniformname);
	if (location == -1)
	{
		std::cout << "WARNING::ShaderProgram::UniformLocation : \""
			<< uniformname << "\" not use or not found." << std::endl;
	}

	return location; 
}

void ShaderProgram::setUniform4f(const char* uniformname, float x, float y, float z, float w) const
{
	int loc = getUniformLocation(uniformname); 
	glUniform4f(loc, x, y, z, w); 
}

void ShaderProgram::setUniform4f(int location, float x, float y, float z, float w) const 
{
	glUniform4f(location, x, y, z, w); 
}

void ShaderProgram::setUniform3f(const char* uniformname, float x, float y, float z) const
{
	int loc = getUniformLocation(uniformname);
	glUniform3f(loc, x, y, z);
}

void ShaderProgram::setUniform3f(int location, float x, float y, float z) const
{
	glUniform3f(location, x, y, z);
}

void ShaderProgram::setUniform1i(const char* name, int location) const 
{
	glUniform1i(this->getUniformLocation(name), location);
}

void ShaderProgram::setUniform1f(const char* uniformname, float data) const
{
	int loc = getUniformLocation(uniformname);
	glUniform1f(loc, data);
}

void ShaderProgram::setMat4(const char* mat4name, const GLfloat* data) const
{
	int loc = getUniformLocation(mat4name);
	// send at location loc , 1 matrix, not in column major order, data
	glUniformMatrix4fv(loc, 1, GL_FALSE, data);
}
