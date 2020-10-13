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
		_gl_type = GL_TEXTURE_1D;
		prefix = "1dm";
		break;
	case TextureType::Texture2D:
		_gl_type = GL_TEXTURE_2D;
		prefix = "2dm";
		break;
	case TextureType::Texture3D:
		_gl_type = GL_TEXTURE_3D;
		prefix = "3dm";
		break;
	}
	createDebugLabel(GL_TEXTURE, prefix);
}

TextureObject::TextureObject() :
	OGL_OBJECT(""), _type(TextureType::Texture2D), _gl_type(GL_TEXTURE_2D)
{
	glGenTextures(1, &_object_name);

	std::string prefix  ;
	createDebugLabel(GL_TEXTURE, "2dm");
}

TextureObject::~TextureObject()
{
	glDeleteTextures(1, &_object_name);
}

void TextureObject::bind(unsigned short unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(_gl_type, _object_name);
}
void TextureObject::release()
{
	glBindTexture(_gl_type, 0);
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

void TextureObject::setWrapping(Wrapping wrap_s_t)
{
	if (_gl_type == GL_TEXTURE_2D )
	{
		GLint param = GL_REPEAT;
		switch (wrap_s_t)
		{
		case Wrapping::Repeat:
			param = GL_REPEAT;
			break;
		case Wrapping::MirroredRepeat:
			param = GL_MIRRORED_REPEAT;
			break;
		case Wrapping::ClampToEdge:
			param = GL_CLAMP_TO_EDGE;
			break;
		}
		glTexParameteri(_gl_type, GL_TEXTURE_WRAP_S, param);
		glTexParameteri(_gl_type, GL_TEXTURE_WRAP_T, param);
	}
}
void TextureObject::setWrapping(Wrapping wrap_s, Wrapping wrap_t)
{
	if (_gl_type == GL_TEXTURE_2D)
	{
		GLint param_s = GL_REPEAT, param_t = GL_REPEAT;
		switch (wrap_s)
		{
		case Wrapping::Repeat:
			param_s = GL_REPEAT;
			break;
		case Wrapping::MirroredRepeat:
			param_s = GL_MIRRORED_REPEAT;
			break;
		case Wrapping::ClampToEdge:
			param_s = GL_CLAMP_TO_EDGE;
			break;
		}
		switch (wrap_t)
		{
		case Wrapping::Repeat:
			param_t = GL_REPEAT;
			break;
		case Wrapping::MirroredRepeat:
			param_t = GL_MIRRORED_REPEAT;
			break;
		case Wrapping::ClampToEdge:
			param_t = GL_CLAMP_TO_EDGE;
			break;
		}
		glTexParameteri(_gl_type, GL_TEXTURE_WRAP_S, param_s);
		glTexParameteri(_gl_type, GL_TEXTURE_WRAP_T, param_t);
	}
}
void TextureObject::setMagFilter(TextureFilter filter)
{
	if (_gl_type == GL_TEXTURE_2D)
	{
		GLint param = GL_LINEAR;
		if (filter == TextureFilter::Nearest)
			param = GL_NEAREST;
		glTexParameteri(_gl_type, GL_TEXTURE_MAG_FILTER, param);
	}

}
void TextureObject::setMinFilter(TextureFilter tex_filter, MipmapFilter mip_filter)
{
	if (_gl_type == GL_TEXTURE_2D)
	{
		GLint param = GL_LINEAR_MIPMAP_LINEAR;
		if (tex_filter == TextureFilter::Linear && mip_filter == MipmapFilter::Nearest)
			param = GL_LINEAR_MIPMAP_NEAREST;
		else if (tex_filter == TextureFilter::Nearest && mip_filter == MipmapFilter::Linear)
			param = GL_NEAREST_MIPMAP_LINEAR;
		else if (tex_filter == TextureFilter::Nearest && mip_filter == MipmapFilter::Nearest)
			param = GL_NEAREST_MIPMAP_NEAREST;


		glTexParameteri(_gl_type, GL_TEXTURE_MAG_FILTER, param);
	}
}

void TextureObject::generateMipmap()
{
	glGenerateMipmap(_gl_type);
}


_ZRV_END