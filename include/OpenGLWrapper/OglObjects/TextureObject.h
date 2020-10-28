#pragma once
#include "../OglBase.h"
#include "OglObject.h"
#include "../Image2D.h"

_ZRV_BEGIN

class TextureObject : public OGL_OBJECT
{
public:
	enum class TextureType { Texture1D, Texture2D, Texture3D };
	enum class Wrapping {Repeat, MirroredRepeat, ClampToEdge};
	enum class TextureFilter { Nearest, Linear };
	enum class MipmapFilter { Nearest, Linear };

	TextureObject(TextureType type, const std::string& obj_label = "");
	TextureObject();

	~TextureObject();
	void bind(unsigned short unit = 0) const;
	void release();
	bool setImage(const Image2D& img);
	void setWrapping(Wrapping wrap_s_t);
	void setWrapping(Wrapping wrap_s, Wrapping wrap_t);
	void setMagFilter(TextureFilter filter);
	void setMinFilter(TextureFilter tex_filter, MipmapFilter mip_filter = MipmapFilter::Linear);
	void generateMipmap();
	std::string getLog()
	{
		return _error_str;
	}
private:
	TextureType _type;
	GLenum _gl_type;
	std::string _error_str;
};

_ZRV_END