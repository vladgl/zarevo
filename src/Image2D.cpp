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
		_channels = 0;
		_data = nullptr;
	}
	else
	{
		_format = Format::L;
		_channels = 1;
		int data_size = _width * _height;
		_data = new GLubyte[data_size];
		for (int i = 0; i < data_size * _channels; i += _channels)
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
		_format = Format::UNSUPPORTED;
		_channels = 0;
		_data = nullptr;
	}
	else
	{
		_format = Format::LA;
		_channels = 2;
		int data_size = _width * _height;
		_data = new GLubyte[data_size * _channels];
		for (int i = 0; i < data_size * _channels; i += _channels) 
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
		_channels = 0;
		_data = nullptr;
	}
	else
	{
		_format = Format::RGB;
		_channels = 3;
		int data_size = _width * _height;

		_data = new GLubyte[data_size * _channels];
		for (int i = 0; i < data_size * _channels; i += _channels)
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
		_channels = 0;
		_data = nullptr;
	}
	else
	{
		_format = Format::RGBA;
		_channels = 4;
		int data_size = _width * _height;
		_data = new GLubyte[data_size * 4];
		for (int i = 0; i < data_size * _channels; i += _channels)
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
	_data = SOIL_load_image(path.c_str(), &_width, &_height, &_channels, SOIL_LOAD_AUTO);
	if (_data == nullptr)
	{
		_width = 0;
		_height = 0; 
		_format = Format::UNSUPPORTED;
		_channels = 0;
		return;
	}
	switch (_channels)
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
	_channels = std::exchange(other._channels, 0);
}
Image2D::~Image2D()
{
	if (_data != nullptr) delete[] _data;
}

const Image2D& Image2D::flipX() const
{
	if (_channels == 0) return (*this);


	for (int texel = 0; texel < _width / 2; ++texel)
		for (int row = 0; row < _height; ++row)
		{
			for (int c = 0; c < _channels; ++c)
			{
				int block = (texel + row * _width) * _channels + c;
				int op_block = (_width - 1 - texel + row * _width) * _channels + c;
				std::swap(_data[block], _data[op_block]);
			}
		}
	return (*this);
}
const Image2D& Image2D::flipY() const
{
	int channels = 0;
	if (_channels == 0) return (*this);
	int dwidth = _width * _channels;
	for (int row = 0; row < _height / 2; ++row)
		for (int col_block = 0; col_block < dwidth; ++col_block)
	    {
	    	std::swap(_data[col_block + row * dwidth], _data[col_block + (_height - 1 - row) * dwidth]);
	    }
	return (*this);
}

_ZRV_END