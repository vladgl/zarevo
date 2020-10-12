#include "../include/OpenGLWrapper/Image2D.h"
#include "SOIL/SOIL.h"
#include <type_traits>
_ZRV_BEGIN

Image2D::Image2D(int width, int height, glm::vec1 color) :
	_width(width), _height(height)
{
	if (_width < 0 || _height < 0)
	{
		_width = 0;
		_height = 0;
		_format = Format::UNSUPPORTED;
		_data = nullptr;
	}
	else
	{
		_format = Format::L;
		int data_size = _width * _height;
		_data = new GLubyte[data_size];
		for (int i = 0; i < data_size; ++i)
		{
			_data[i] = glm::clamp(static_cast<GLubyte>(color.r * 255), GLubyte(0), GLubyte(255));
		}
	}

}
Image2D::Image2D(int width, int height, glm::vec2 color) :
	_width(width), _height(height)
{
	if (_width < 0 || _height < 0)
	{
		_width = 0;
		_height = 0;
		_data = nullptr;
		_format = Format::UNSUPPORTED;
	}
	else
	{
		_format = Format::LA;
		int data_size = _width * _height;
		_data = new GLubyte[data_size * 2];
		for (int i = 0; i < data_size; ++i)
		{
			_data[i] = glm::clamp(static_cast<GLubyte>(color.r * 255), GLubyte(0), GLubyte(255));
			_data[i + 1] = glm::clamp(static_cast<GLubyte>(color.g * 255), GLubyte(0), GLubyte(255));
		}
	}

}
Image2D::Image2D(int width, int height, glm::vec3 color) :
	_width(width), _height(height)
{
	if (_width < 0 || _height < 0)
	{
		_width = 0;
		_height = 0;
		_format = Format::UNSUPPORTED;
		_data = nullptr;
	}
	else
	{
		_format = Format::RGB;
		int data_size = _width * _height;
		_data = new GLubyte[data_size * 3];
		for (int i = 0; i < data_size; ++i)
		{
			_data[i] = glm::clamp(static_cast<GLubyte>(color.r * 255), GLubyte(0), GLubyte(255));
			_data[i + 1] = glm::clamp(static_cast<GLubyte>(color.g * 255), GLubyte(0), GLubyte(255));
			_data[i + 2] = glm::clamp(static_cast<GLubyte>(color.b * 255), GLubyte(0), GLubyte(255));
		}
	}

}

Image2D::Image2D(int width, int height, glm::vec4 color) :
	_width(width), _height(height)
{
	if (_width < 0 || _height < 0)
	{
		_width = 0;
		_height = 0;
		_format = Format::UNSUPPORTED;
		_data = nullptr;
	}
	else
	{
		_format = Format::RGBA;
		int data_size = _width * _height * 4;
		_data = new GLubyte[data_size * 4];
		for (int i = 0; i < data_size; ++i)
		{
			_data[i] = glm::clamp(static_cast<GLubyte>(color.r * 255), GLubyte(0), GLubyte(255));
			_data[i + 1] = glm::clamp(static_cast<GLubyte>(color.g * 255), GLubyte(0), GLubyte(255));
			_data[i + 2] = glm::clamp(static_cast<GLubyte>(color.b * 255), GLubyte(0), GLubyte(255));
			_data[i + 3] = glm::clamp(static_cast<GLubyte>(color.a * 255), GLubyte(0), GLubyte(255));
		}
	}
}

Image2D::Image2D(const std::string& path)
{
	int channels;
	_data = SOIL_load_image(path.c_str(), &_width, &_height, &channels, SOIL_LOAD_AUTO);
	if (_data == nullptr)
	{
		_width = 0;
		_height = 0; 
		_format = Format::UNSUPPORTED;
		return;
	}
	switch (channels)
	{
	case 1 :
		_format = Format::L;
		break;
	case 2:
		_format = Format::LA;
		break;
	case 3:
		_format = Format::RGB;
		break;
	case 4:
		_format = Format::RGBA;
		break;
	default: 
		_format = Format::UNSUPPORTED;
	}
}
Image2D::Image2D(Image2D&& other) noexcept
{
	_data = std::exchange(other._data, nullptr);
	_width = std::exchange(other._width, 0);
	_height = std::exchange(other._height, 0);
	_format = std::exchange(other._format, Format::UNSUPPORTED);
}
Image2D::~Image2D()
{
	if (_data != nullptr) delete[] _data;
}

const Image2D& Image2D::flipX() const
{
	int channels = 0;
	if (_format != Format::UNSUPPORTED)
	{
		switch (_format)
		{
		case Format::L:
			channels = 1;
			break;
		case Format::LA:
			channels = 2;
			break;
		case Format::RGB:
			channels = 3;
			break;
		case Format::RGBA:
			channels = 4;
			break;
		}
	}
	else return (*this);


	for (int texel = 0; texel < _width / 2; ++texel)
		for (int row = 0; row < _height; ++row)
		{
			for (int c = 0; c < channels; ++c)
			{
				int block = (texel + row * _width) * channels + c;
				int op_block = (_width - 1 - texel + row * _width) * channels + c;
				std::swap(_data[block], _data[op_block]);
			}
		}
	return (*this);
}
const Image2D& Image2D::flipY() const
{
	int channels = 0;
	if (_format != Format::UNSUPPORTED)
	{
		switch (_format)
		{
		case Format::L:
			channels = 1;
			break;
		case Format::LA:
			channels = 2;
			break;
		case Format::RGB:
			channels = 3;
			break;
		case Format::RGBA:
			channels = 4;
			break;
		}
	}
	else return (*this);
	int dwidth = _width * channels;
	for (int row = 0; row < _height / 2; ++row)
		for (int col_block = 0; col_block < dwidth; ++col_block)
	    {
	    	std::swap(_data[col_block + row * dwidth], _data[col_block + (_height - 1 - row) * dwidth]);
	    }
	return (*this);
}

_ZRV_END