#pragma once
#include "../OglBase.h"
#include "OglObject.h"
#include "../Image2D.h"

_ZRV_BEGIN

class TextureObject : public OGL_OBJECT
{
public:
	enum class TextureType { Texture1D, Texture2D, Texture3D };
	TextureObject(TextureType type, const std::string& obj_label = "");
	TextureObject();

	~TextureObject();
	void bind();
	void release();
	bool setImage(const Image2D& img);
	std::string getLog()
	{
		return _error_str;
	}
private:
	TextureType _type;
	std::string _error_str;
};

_ZRV_END