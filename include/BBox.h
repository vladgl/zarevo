#pragma once
#include "zarevo_base.h"

_ZRV_BEGIN

extern class SimpleMesh;

struct AxisAlignedBB
{
	friend class SimpleMesh;
	AxisAlignedBB();
	glm::vec3 _bleft;
	glm::vec3 _tright;
	glm::vec3 _center;
	void createBbox(const SimpleMesh& mesh);
};

_ZRV_END