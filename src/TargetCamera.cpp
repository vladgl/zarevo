#include "../include/TargetCamera.h"
#include <iostream>
_ZRV_SOURCE



/*! \brief Returns transition matrix to {x, y, z} coordinate system with center in p;
*note that triple {x, y, z} should be normalized and orthogonal 
*/
glm::mat4 transitionMatrix(const glm::vec3& x, const glm::vec3& y, const glm::vec3& z, const glm::vec3& p);

// Just an operator overload to print glm::vec3
std::ostream& operator<< (std::ostream& out, glm::vec3 vec)
{
	out << vec.x << " " << vec.y << " " << vec.z;
	return out;
}


void TargetCamera::resetVectorsGS()
{
	_direction = glm::normalize(_direction);
	_right = glm::normalize(glm::cross(_direction, _up));
	_up = _up - glm::dot(_up, _direction) / glm::dot(_direction, _direction) * _direction;
	_up = glm::normalize(_up);
	_rolled_up = _up;
}

glm::mat3 TargetCamera::getRotationMatrix()
{
	return glm::mat3(getViewMatrix());
}


TargetCamera::TargetCamera() :
	_eye(0.0f, -1.0f, 0.0f), _center(0.0f, 0.0f, 0.0f), _up(0.0f, 0.0f, 1.0f), _label(""), _fov(glm::radians(80.0f))
{
	_direction = glm::normalize(_center - _eye); 
	resetVectorsGS();
	_dpos = glm::vec3(0.0f, 0.0f, 0.0f);
	_dir_offset = 1.0;
	_length = glm::length(_eye - _center);
}

TargetCamera::TargetCamera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, const std::string& label) :
	_eye(eye), _center(target), _up(glm::normalize(up)), _label(label), _fov(glm::radians(80.0f))
{	
	_direction = glm::normalize(_center - _eye);
	resetVectorsGS();
	_dpos = glm::vec3(0.0f, 0.0f, 0.0f);
	_dir_offset = 1.0;
	_length = glm::length(_eye - _center);
}


void TargetCamera::rotateAroundUpV(double yaw, double pitch)
{
	float r_yaw = yaw;
	// if camera is upside down we inverse yaw
	if (glm::dot(_up, _rolled_up) < 0.0f)
	{
		r_yaw = -yaw;
	}
	_direction = _direction * glm::angleAxis(float(r_yaw), _rolled_up);
	_up = _up * glm::angleAxis(float(r_yaw), _rolled_up);
	_right = glm::cross(_direction, _up);

	_up = _up * glm::angleAxis(float(pitch), _right);
	_direction = _direction * glm::angleAxis(float(pitch), _right);

	_eye = _center - _direction * _length;
}

void TargetCamera::rollUpV(double roll)
{
	_rolled_up = _rolled_up * glm::angleAxis(float(roll), _direction);
	_up = _up * glm::angleAxis(float(roll), _direction);
	_right = _right * glm::angleAxis(float(roll), _direction);
	_eye = _center - _direction * _length;
}


glm::mat4 TargetCamera::getViewMatrix()
{
	glm::mat4 view{ 1.0f };

	view = transitionMatrix(_right, _up, -_direction, _eye);
	return view;
}


void TargetCamera::parallelOffset(double right, double up)
{
	glm::mat4 view = getViewMatrix();
	//view^-1 == view^T, so columns is rows; note that glm::mat4 is column-major
	glm::vec3 vRight = { view[0][0], view[1][0], view[2][0] };
	glm::vec3 vUp = { view[0][1], view[1][1], view[2][1] };

	glm::vec3 dpos = float(right) * vRight + float(up) * vUp;
	_eye += dpos;
	_center += dpos;
}


void TargetCamera::dirOffset(double offset)
{
	if (_length * offset > 0.0001)
	{
		_eye = _center - _direction * _length * float(offset);
		_length = glm::length(_eye - _center);
	}
}



void TargetCamera::fitInView(const AxisAlignedBB& bbox)
{
	_center = bbox._center;
	glm::mat3 rotation = getRotationMatrix();
	glm::vec3 v = rotation * (bbox._bleft - bbox._tright);
	float lin = glm::length(v);

	float d = lin / 2.0 / glm::tan(_fov / 2.0);
	_eye = _center - _direction * d;
	_length = glm::length(_eye - _center);
//	_dir_offset = d / glm::length(_eye - _center) ;
}


void TargetCamera::setEyeV(const glm::vec3& new_eye)
{
	_eye = new_eye;
	_direction = glm::normalize(_center - _eye);
	resetVectorsGS();
	_length = glm::length(_eye - _center);
}
void TargetCamera::setTargerV(const glm::vec3& new_target)
{
	_center = new_target;
	_direction = glm::normalize(_center - _eye);
	resetVectorsGS();
	_length = glm::length(_eye - _center);
}
void TargetCamera::setUpV(const glm::vec3& new_up)
{
	_up = glm::normalize(new_up);
	resetVectorsGS();
}

void TargetCamera::setFov(float new_fov)
{
	_fov = new_fov;
}


glm::vec3 TargetCamera::eye()
{
	return _eye;
}
glm::vec3 TargetCamera::target()
{
	return _center;
}
glm::vec3 TargetCamera::up()
{
	return _up;
}
float TargetCamera::fov()
{
	return _fov;
}

float TargetCamera::length()
{
	return glm::length(_center - _eye) * _dir_offset;
}


glm::mat4 transitionMatrix(const glm::vec3& x, const glm::vec3& y, const glm::vec3& z, const glm::vec3& p)
{
	glm::mat4 tr{ 1.0f };
	tr[0][0] = x[0];
	tr[1][0] = x[1];
	tr[2][0] = x[2];

	tr[0][1] = y[0];
	tr[1][1] = y[1];
	tr[2][1] = y[2];

	tr[0][2] = z[0];
	tr[1][2] = z[1];
	tr[2][2] = z[2];

	tr[3][0] = -glm::dot(p, x);
	tr[3][1] = -glm::dot(p, y);
	tr[3][2] = -glm::dot(p, z);

	return tr;
}