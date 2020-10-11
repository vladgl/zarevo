#pragma once
#include "Shader.h"
#include "OglObject.h"
_ZRV_BEGIN
class ShaderProgram : public OGL_OBJECT
{
public:
	ShaderProgram(const std::string& obj_label = "");
	~ShaderProgram();
	bool init(Shader* vertex_s, Shader* fragment_s);
	bool init(const char* v_path, const char* f_path);
	bool attachVertexShader(Shader* vs);
	bool attachFragmentShader(Shader* fs);
	bool detachVertexShader(Shader* vs);
	bool detachFragmentShader(Shader* fs);
	bool link();


	void use();
	void release();
	GLint getUnifLoc(const GLchar* name);
	GLint getAttribLoc(const GLchar* name);

	std::string getLog()
	{
		return _error_str;
	}

private:

	bool procError();

	std::string _error_str;
};

_ZRV_END