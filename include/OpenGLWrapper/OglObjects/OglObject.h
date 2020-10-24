#pragma once
#include "../OglBase.h"
_ZRV_BEGIN
class OGL_OBJECT
{
public:
	OGL_OBJECT(const std::string& obj_label = "") :
		_label(obj_label), _object_name(0)
	{

	}
	virtual GLuint objectName()
	{
		return _object_name;
	}

	virtual std::string debugLabel()
	{
		return _debug_label;
	}
	
	virtual std::string objectLabel()
	{
		return _label;
	}

protected:

	virtual std::string createDebugLabel(GLenum typeId, const std::string& obj_prefix = "")
	{
		_debug_label = 
			"zrv_" + 
			getObjectTypeNameByIdentifier(typeId, obj_prefix) +
			"_" + 
		     _label + ((_label.size() != 0) ? "_" : "") +
			std::to_string(_object_name);
		glObjectLabel(typeId, _object_name, _debug_label.size() + 1, _debug_label.c_str());
		return _debug_label;
	}

	GLuint _object_name;
	//label set by glObjectLabel() for debugging purposes
	std::string _debug_label;
	//part of _debug_label specified by user
	std::string _label;
private:
	OGL_OBJECT() = delete;
};

_ZRV_END

