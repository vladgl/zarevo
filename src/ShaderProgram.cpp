#include "../include/OpenGLWrapper/OglObjects/ShaderProgram.h"

_ZRV_SOURCE

ShaderProgram::ShaderProgram(const std::string& obj_label) :
	OGL_OBJECT(obj_label)
{
	_object_name = glCreateProgram();
	createDebugLabel(GL_PROGRAM);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(_object_name);
}

bool ShaderProgram::init(Shader* vertex_s, Shader* fragment_s)
{
	attachVertexShader(vertex_s);
	attachFragmentShader(fragment_s);

	return link();
}

bool ShaderProgram::init(const char* v_path, const char* f_path)
{
	Shader* vertex_s = new Shader(Shader::ShaderType::VertexShader);
	Shader* fragment_s = new Shader(Shader::ShaderType::FragmentShader);

	vertex_s->setSourceFromFile(v_path);
	fragment_s->setSourceFromFile(f_path);

	attachVertexShader(vertex_s);
	attachFragmentShader(fragment_s);

	if (!vertex_s->compile())
	{
		_error_str = vertex_s->getLog();
		delete vertex_s;
		delete fragment_s;
		return false;
	}
	if (!fragment_s->compile())
	{
		_error_str = fragment_s->getLog();
		delete vertex_s;
		delete fragment_s;
		return false;
	}
	delete vertex_s;
	delete fragment_s;
	return link();
}

bool ShaderProgram::attachVertexShader(Shader* vs)
{
	glAttachShader(_object_name, vs->objectName());
	return true;
}
bool ShaderProgram::attachFragmentShader(Shader* fs)
{
	glAttachShader(_object_name, fs->objectName());
	return true;

}
bool ShaderProgram::detachVertexShader(Shader* vs)
{
	glDetachShader(_object_name, vs->objectName());
	return true;
}
bool ShaderProgram::detachFragmentShader(Shader* fs)
{
	glDetachShader(_object_name, fs->objectName());
	return true;
}
bool ShaderProgram::link()
{
	glLinkProgram(_object_name);
	return procError();
}

void ShaderProgram::use() const
{
	glUseProgram(_object_name);
}

void ShaderProgram::release() const
{
	glUseProgram(0);
}

GLint ShaderProgram::getUnifLoc(const GLchar* name) const
{
	GLint loc;
	loc = glGetUniformLocation(_object_name, name);
	return loc;
}

GLint ShaderProgram::getAttribLoc(const GLchar* name) const
{
	GLint loc;
	loc = glGetAttribLocation(_object_name, name);
	return loc;
}

bool ShaderProgram::procError()
{
	GLint success;

	glGetProgramiv(_object_name, GL_LINK_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(_object_name, 512, nullptr, infoLog);
		_error_str = std::string(infoLog);
		return false;;
	}

	return true;
}