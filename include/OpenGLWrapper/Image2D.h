#pragma once
#include "OglBase.h"
#include <glm/glm.hpp>

_ZRV_BEGIN

class Image2D
{
public:
	enum class Format {L, LA, RGB, RGBA, UNSUPPORTED};
	//L image
	Image2D(int width, int height, glm::vec1 color);
	//LA image
	Image2D(int width, int height, glm::vec2 color);
	//RGB image
	Image2D(int width, int height, glm::vec3 color);
	//RGBA image
	Image2D(int width, int height, glm::vec4 color);
	Image2D(const std::string& path);
	Image2D(Image2D&&) noexcept;
	~Image2D();

	int width() const
	{
		return _width;
	}
	int height() const
	{
		return _height;
	}
	
	const GLubyte* rawData() const
	{
		return _data;
	}
	Format format() const
	{
		return _format;
	}

	int channels() const
	{
		return _channels;
	}

	const Image2D& flipX() const;
	const Image2D& flipY() const;

private:
	GLubyte* _data;
	Format _format;
	int _channels;
	int _width, _height;

	Image2D() = delete;
	Image2D(const Image2D&) = delete;
	Image2D& operator=(const Image2D&) = delete;
};

_ZRV_END