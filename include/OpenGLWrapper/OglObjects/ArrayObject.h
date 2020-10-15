#pragma once
#include "../OglBase.h"
#include "OglObject.h"

_ZRV_BEGIN
class ArrayObject : public OGL_OBJECT
{
public:
	ArrayObject(const std::string& obj_label = "");
	~ArrayObject();

	void bind() const;
	void release() const;
};

_ZRV_END
