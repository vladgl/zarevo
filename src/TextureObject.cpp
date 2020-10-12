#include "../include/OpenGLWrapper/OglObjects/TextureObject.h"

_ZRV_BEGIN

TextureObject::TextureObject(TextureType type, const std::string& obj_label) :
	OGL_OBJECT(obj_label), _type(type)
{
	glGenTextures(1, &_object_name);
	std::string prefix;
	switch (_type)
	{
	case TextureType::Texture1D:
		prefix = "1dm";
		break;
	case TextureType::Texture2D:
		prefix = "2dm";
		break;
	case TextureType::Texture3D:
		prefix = "3dm";
		break;
	}
	createDebugLabel(GL_TEXTURE, prefix);
}

TextureObject::TextureObject() :
	OGL_OBJECT(""), _type(TextureType::Texture2D)
{
	glGenTextures(1, &_object_name);

	std::string prefix  ;
	createDebugLabel(GL_TEXTURE, "2dm");
}

TextureObject::~TextureObject()
{
	glDeleteTextures(1, &_object_name);
}

void TextureObject::bind()
{
	GLenum type;
	switch (_type)
	{
	case TextureType::Texture1D:
		type = GL_TEXTURE_1D;
		break;
	case TextureType::Texture2D:
		type = GL_TEXTURE_2D;
		break;
	case TextureType::Texture3D:
		type = GL_TEXTURE_3D;
		break;
	}
	glBindTexture(type, _object_name);
}
void TextureObject::release()
{
	GLenum type;
	switch (_type)
	{
	case TextureType::Texture1D:
		type = GL_TEXTURE_1D;
		break;
	case TextureType::Texture2D:
		type = GL_TEXTURE_2D;
		break;
	case TextureType::Texture3D:
		type = GL_TEXTURE_3D;
		break;
	}
	glBindTexture(type, 0);
}

bool TextureObject::setImage(const Image2D& img)
{
	GLenum format = GL_LUMINANCE;
	if (img.width() < 64 || img.height() < 64)
	{
		_error_str = "Image size is too small (width and height must be at least 64 pixels)";
		return false;
	}
	if (img.format() == Image2D::Format::UNSUPPORTED)
	{
		_error_str = "Texture format is unsupproted";
		return false;
	}
	switch (img.format())
	{
	case Image2D::Format::L:
		format = GL_LUMINANCE;
		break;
	case Image2D::Format::LA:
		format = GL_LUMINANCE_ALPHA;
		break;
	case Image2D::Format::RGB:
		format = GL_RGB;
		break;
	case Image2D::Format::RGBA:
		format = GL_RGBA;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, img.width(), img.height(), 0, format, GL_UNSIGNED_BYTE, img.rawData());

}

_ZRV_END