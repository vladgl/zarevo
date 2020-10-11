#pragma once
#include "../OglBase.h"
#include "OglObject.h"

_ZRV_BEGIN
class ArrayObject : public OGL_OBJECT
{
public:
	ArrayObject(const std::string& obj_label = "");
	~ArrayObject();

	void bind();
	void release();
};

_ZRV_END
