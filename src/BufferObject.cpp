#include "../include/OpenGLWrapper/OglObjects/BufferObject.h"

_ZRV_SOURCE

BufferObject::BufferObject(BufferType type, const std::string& obj_label) :
	OGL_OBJECT(obj_label),
	_type(type)
{
	glGenBuffers(1, &_object_name);
	// \todo: check label size (no more then 258)

	std::string prefix;
	switch (_type)
	{
	case BufferType::Array:
		prefix = "Arr";
		break;
	case BufferType::ElementArray:
		prefix = "Elm";
		break;
	default:
		prefix = "Bad";
		break;
	}
	createDebugLabel(GL_BUFFER, prefix);
}
BufferObject::~BufferObject()
{
	glDeleteBuffers(1, &_object_name);
}

void BufferObject::bind()
{
	switch (_type)
	{
	case BufferType::Array:
		glBindBuffer(GL_ARRAY_BUFFER, _object_name);
		break;
	case BufferType::ElementArray:
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _object_name);
		break;
	default:
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void BufferObject::release()
{
	switch (_type)
	{
	case BufferType::Array:
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	case BufferType::ElementArray:
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		break;
	default: break;
	}
}

template<typename T>
void BufferObject::allocate(const std::vector<T>& data, DrawMode mode)
{
	GLenum gl_mode;
	switch (mode)
	{
	case DrawMode::Static:
		gl_mode = GL_STATIC_DRAW;
		break;
	case DrawMode::Dynamic:
		gl_mode = GL_DYNAMIC_DRAW;
		break;
	case DrawMode::Stream:
		gl_mode = GL_STREAM_DRAW;
		break;
	default:
		gl_mode = GL_STATIC_DRAW;
	}
	switch (_type)
	{
	case BufferType::Array:
		glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), &data[0], gl_mode);
		break;
	case BufferType::ElementArray:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T) * data.size(), &data[0], gl_mode);
		break;
	default: break;
	}
}

template<typename T>
void BufferObject::subData(GLuint offset, const std::vector<T>& data)
{
	switch (_type)
	{
	case BufferType::Array:
		glBufferSubData(GL_ARRAY_BUFFER, offset, data.size() * sizeof(T), &data[0]);
		break;
	case BufferType::ElementArray:
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, data.size() * sizeof(T), &data[0]);
		break;
	}
}


// explicit template definitions for common OpenGL types

template void BufferObject::allocate<GLboolean> (const std::vector<GLboolean>& data, DrawMode mode);
template void BufferObject::allocate<GLbitfield>(const std::vector<GLbitfield>& data, DrawMode mode);
template void BufferObject::allocate<GLbyte>    (const std::vector<GLbyte>& data, DrawMode mode);
template void BufferObject::allocate<GLshort>   (const std::vector<GLshort>& data, DrawMode mode);
template void BufferObject::allocate<GLint>     (const std::vector<GLint>& data, DrawMode mode);
template void BufferObject::allocate<GLsizei>   (const std::vector<GLsizei>& data, DrawMode mode);
template void BufferObject::allocate<GLubyte>   (const std::vector<GLubyte>& data, DrawMode mode);
template void BufferObject::allocate<GLushort>  (const std::vector<GLushort>& data, DrawMode mode);
template void BufferObject::allocate<GLuint>    (const std::vector<GLuint>& data, DrawMode mode);
template void BufferObject::allocate<GLfloat>   (const std::vector<GLfloat>& data, DrawMode mode);
template void BufferObject::allocate<GLclampf>  (const std::vector<GLclampf>& data, DrawMode mode);
template void BufferObject::allocate<GLdouble>  (const std::vector<GLdouble>& data, DrawMode mode);
template void BufferObject::allocate<GLclampd>  (const std::vector<GLclampd>& data, DrawMode mode);
template void BufferObject::allocate<GLchar>    (const std::vector<GLchar>& data, DrawMode mode);


template void BufferObject::subData<GLboolean>(GLuint offset, const std::vector<GLboolean>& data);
template void BufferObject::subData<GLbitfield>(GLuint offset, const std::vector<GLbitfield>& data);
template void BufferObject::subData<GLbyte>(GLuint offset, const std::vector<GLbyte>& data);
template void BufferObject::subData<GLshort>(GLuint offset, const std::vector<GLshort>& data);
template void BufferObject::subData<GLint>(GLuint offset, const std::vector<GLint>& data);
template void BufferObject::subData<GLsizei>(GLuint offset, const std::vector<GLsizei>& data);
template void BufferObject::subData<GLubyte>(GLuint offset, const std::vector<GLubyte>& data);
template void BufferObject::subData<GLushort>(GLuint offset, const std::vector<GLushort>& data);
template void BufferObject::subData<GLuint>(GLuint offset, const std::vector<GLuint>& data);
template void BufferObject::subData<GLfloat>(GLuint offset, const std::vector<GLfloat>& data);
template void BufferObject::subData<GLclampf>(GLuint offset, const std::vector<GLclampf>& data);
template void BufferObject::subData<GLdouble>(GLuint offset, const std::vector<GLdouble>& data);
template void BufferObject::subData<GLclampd>(GLuint offset, const std::vector<GLclampd>& data);
template void BufferObject::subData<GLchar>(GLuint offset, const std::vector<GLchar>& data);
