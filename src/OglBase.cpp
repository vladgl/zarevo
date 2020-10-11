#include "../include/OpenGLWrapper/OglBase.h"
#include <map>

_ZRV_BEGIN
// Debugging information
const std::map<GLenum, std::string> id_typename_map
{
	{GL_BUFFER, "Buff"},
	{GL_SHADER, "Shdr"},
	{GL_PROGRAM, "ShdProg"},
	{GL_VERTEX_ARRAY, "ArrObjt"},
	{GL_QUERY, "QueObjt"},
	{GL_PROGRAM_PIPELINE, "PrgPipe"},
	{GL_TRANSFORM_FEEDBACK, "TrnFbck"},
	{GL_SAMPLER, "SmpObjt"},
	{GL_TEXTURE, "Texr"},
	{GL_RENDERBUFFER, "RndBuff"},
	{GL_FRAMEBUFFER, "FrmBuff"}
};

zrv::ErrorList	glErrorHandler()
{
	GLenum err;
	ErrorList lst;
	std::string str;
	while ((err = glGetError()) != GL_NO_ERROR)
		switch (err)
		{
		case GL_INVALID_ENUM:
			lst.push_back(std::string("GL_INVALID_ENUM"));
			break;
		case GL_INVALID_VALUE:
			lst.push_back(std::string("GL_INVALID_VALUE"));
			break;
		case GL_INVALID_OPERATION:
			lst.push_back(std::string("GL_INVALID_OPERATION"));
			break;
		case GL_STACK_OVERFLOW:
			lst.push_back(std::string("GL_STACK_OVERFLOW"));
			break;
		case GL_STACK_UNDERFLOW:
			lst.push_back(std::string("GL_STACK_UNDERFLOW"));
			break;
		case GL_OUT_OF_MEMORY:
			lst.push_back(std::string("GL_OUT_OF_MEMORY"));
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			lst.push_back(std::string("GL_INVALID_FRAMEBUFFER_OPERATION"));
			break;
		case GL_CONTEXT_LOST:
			lst.push_back(std::string("GL_CONTEXT_LOST"));
			break;
		default:
			lst.push_back(std::string("Unknown error"));
			break;
		}
	return lst;
}

std::string getObjectTypeNameByIdentifier(GLenum identifier, const std::string& prefix)
{
	std::string out_str = prefix + id_typename_map.at(identifier);
	return out_str;
}

_ZRV_END