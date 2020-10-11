#include "../include/OpenGLWrapper/OglObjects/Shader.h"
#include <sstream>
#include <fstream>

_ZRV_SOURCE

Shader::Shader(ShaderType shader_type, const std::string& obj_label) :
	OGL_OBJECT(obj_label),
	_shader_type(shader_type)
{
	GLenum type;
	std::string prefix;
	switch (_shader_type)
	{
	case ShaderType::VertexShader:
		type = GL_VERTEX_SHADER;
		prefix = "Vrt";
		break;
	case ShaderType::FragmentShader:
		type = GL_FRAGMENT_SHADER;
		prefix = "Frg";
		break;
	case ShaderType::GeometryShader:
		type = GL_GEOMETRY_SHADER;
		prefix = "Geo";
		break;
	default:
		type = GL_VERTEX_SHADER;
		prefix = "Vrt";
	}
	_object_name = glCreateShader(type);
	createDebugLabel(GL_SHADER, prefix);
}

Shader::~Shader()
{
	glDeleteShader(_object_name);
}

bool Shader::setSource(const char** source)
{
	glShaderSource(_object_name, 1, source, nullptr);
	return true;
}

bool Shader::setSourceFromFile(const char* path)
{
	std::ifstream in(path, std::ios::in);
	if (!in.is_open())
	{
		_error_str = "Failed to open file ";
		_error_str += path;
		return false;
	}
	std::stringstream file_data;
	file_data << in.rdbuf();
	std::string tmp_str  = file_data.str();
	const char* source = tmp_str.c_str();
	glShaderSource(_object_name, 1, &source, nullptr);
	in.close();
	return true;
}

bool Shader::compile()
{
	glCompileShader(_object_name);
	GLint status;
	glGetShaderiv(_object_name, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(_object_name, 512, nullptr, infoLog);
		_error_str = std::string(infoLog);
		return false;
	}
	return true;
}