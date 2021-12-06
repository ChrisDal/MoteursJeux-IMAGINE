#include "Shader.h"

Shader::Shader()
	: m_valid(false), m_shader(0), m_source(nullptr)
{};

Shader::Shader(const char* filepath, GLenum shaderType)
	: m_valid(false), m_source(nullptr), m_type(shaderType)
{
	bool loaded = loadSourceShader(filepath);

	if (loaded) {
		m_source = m_ssource.c_str();
		m_shader = glCreateShader(m_type);
		glShaderSource(m_shader, 1, &m_source, NULL);
		glCompileShader(m_shader);
	}

}

bool Shader::loadSourceShader(const char* filepath)
{
	std::ifstream streamfile;
	streamfile.open(filepath, std::ios::in);
	m_ssource = std::string(); // empty source 
	
	if (streamfile.is_open())
	{
		// handling properly string 
		std::stringstream sstr; 
		sstr << streamfile.rdbuf(); 
		m_ssource = sstr.str();

		streamfile.close();

		return true; 
	}

	return false;
	
}

unsigned int Shader::getShader() const
{
	return m_shader;
}

bool Shader::checkValidity()
{
	int  success;
	char infoLog[512];
	// OK = success 0
	glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(m_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::"  
				<<  (m_type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
				<<  "::COMPILATION_FAILED" << std::endl; 
		std::cout << infoLog << std::endl;

		glDeleteShader(m_shader); 
	}

	return bool(success); 
}

Shader::~Shader()
{
	glDeleteShader(m_shader); 
}
