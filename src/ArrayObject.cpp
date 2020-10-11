#include "../include/OpenGLWrapper/OglObjects/ArrayObject.h"

_ZRV_SOURCE

ArrayObject::ArrayObject(const std::string& obj_label) :
	OGL_OBJECT(obj_label)
{
	glGenVertexArrays(1, &_object_name);
	createDebugLabel(GL_VERTEX_ARRAY);
}

ArrayObject::~ArrayObject()
{
	glDeleteVertexArrays(1, &_object_name);
}

void ArrayObject::bind()
{
	glBindVertexArray(_object_name);
}

void ArrayObject::release()
{
	glBindVertexArray(0);
}
