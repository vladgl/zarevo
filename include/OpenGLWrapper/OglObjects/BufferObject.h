#pragma once
#include "../OglBase.h"
#include "OglObject.h"

_ZRV_BEGIN
class BufferObject : public OGL_OBJECT
{
public:
	enum class BufferType { Array, ElementArray };
	enum class DrawMode { Static, Dynamic, Stream };

	BufferObject(BufferType type, const std::string& obj_label = "");
	~BufferObject();

	void bind();
	void release();

	template<typename T>
	void allocate(const std::vector<T>& data, DrawMode mode = DrawMode::Static);
	template<typename T>
	void subData(GLuint offset, const std::vector<T>& data);


private:
	BufferObject() = delete;
	BufferType _type;
};

_ZRV_END