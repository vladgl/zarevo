#include "../include/SimpleMesh.h"
#include "../include/BBox.h"


_ZRV_SOURCE

AxisAlignedBB::AxisAlignedBB() :
	_bleft{0.0f},
    _tright{1.0f},
    _center{0.5f}
{

}
glm::vec3 _bleft;
glm::vec3 _tright;
glm::vec3 _center;


void AxisAlignedBB::createBbox(const SimpleMesh& mesh)
{
	glm::vec4 temp;
	temp.x = mesh._vertices[0];
	temp.y = mesh._vertices[1];
	temp.z = mesh._vertices[2];
	temp = mesh._model_matrix * temp;

	float xmin = temp.x;
    float ymin = temp.y;
    float zmin = temp.z;
	float xmax = temp.x;
	float ymax = temp.y;
	float zmax = temp.z;


	for (int i = 0; i < mesh._vertices.size(); i += 3)
	{
		temp.x = mesh._vertices[i];
		temp.y = mesh._vertices[i + 1];
		temp.z = mesh._vertices[i + 2];
		temp = mesh._model_matrix * temp;

		if (temp.x < xmin)
			xmin = temp.x;
		else if (temp.x > xmax)
			xmax = temp.x;

		if (temp.y < ymin)
			ymin = temp.y;
		else if (temp.y > ymax)
			ymax = temp.y;

		if (temp.z < zmin)
			zmin = temp.z;
		else if (temp.z > zmax)
			zmax = temp.z;
	}

	_bleft = glm::vec3{ xmin, ymin, zmin };
	_tright = glm::vec3{ xmax, ymax, zmax };
	_center = 0.5f * (_bleft + _tright);
}