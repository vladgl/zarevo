#pragma once
#include "../OglBase.h"
#include "OglObject.h"
_ZRV_BEGIN

class Shader : public OGL_OBJECT
{
public:
	enum class ShaderType { VertexShader, FragmentShader, GeometryShader };
	friend class ShaderProgram;
	Shader(ShaderType shader_type, const std::string& obj_label = "");
	~Shader();

	bool setSource(const char** src);
	bool setSourceFromFile(const char* path);
	bool compile();

	std::string getLog() const
	{
		return _error_str;
	}


private:
	Shader() = delete;
	ShaderType _shader_type;

	std::string _error_str;
};

_ZRV_END