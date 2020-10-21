#include "../include/TargetCamera.h"
#include <iostream>
_ZRV_SOURCE

TargetCamera::TargetCamera() :
	_eye(1.0f, 5.0f, 0.0f), _target(0.0f, 0.0f, 0.0f), _up(0.0f, 0.0f, 1.0f), _label(""), _fov(glm::radians(80.0f))
{
	_length = glm::length(_eye - _target);
//	recalcOffsets();
	xz_offset = 0.0f;
	xy_offset = 0.0f;
	_rotation = glm::angleAxis(float(xz_offset), _up);
	glm::vec3 direction = _target - _eye;
	_right = glm::normalize(glm::cross(direction, _up));
	_rotation = glm::angleAxis(float(xy_offset), glm::normalize(glm::cross(direction, _up))) * _rotation;
}

TargetCamera::TargetCamera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, const std::string& label) :
	_eye(eye), _target(target), _up(glm::normalize(up)), _label(label), _fov(glm::radians(80.0f))
{	

	_length = glm::length(_eye - _target);
//	recalcOffsets();
	xz_offset = 0.0f;
	xy_offset = 0.0f;
	_rotation = glm::angleAxis(float(xz_offset), _up);
	glm::vec3 direction = _target - _eye;
	_right = glm::normalize(glm::cross(direction, _up));
	_rotation = glm::angleAxis(float(xy_offset), glm::normalize(glm::cross(direction, _up))) * _rotation;
}

glm::mat4 TargetCamera::getViewMatrix()
{
	glm::vec3 eye = (_eye - _target) * _rotation + _target;
	glm::vec3 vec{ eye.x, _target.y - eye.y, eye.z };
	glm::mat4 view{ 1.0f };
	if (xy_offset > glm::radians(89.9) && xy_offset < glm::radians(271.1))
		view = glm::lookAt(vec, _target, -_up);
	else
		view = glm::lookAt(vec, _target, _up);
	return view;
}

void TargetCamera::setEyeV(const glm::vec3& new_eye)
{
	_eye = new_eye;
	_length = glm::length(_eye - _target);
	xz_offset = 0.0f;
	xy_offset = 0.0f;
	recalcOffsets();
}
void TargetCamera::setTargerV(const glm::vec3& new_target)
{
	_target = new_target;
	_length = glm::length(_eye - _target);
	xz_offset = 0.0f;
	xy_offset = 0.0f;
	recalcOffsets();
}
void TargetCamera::setUpV(const glm::vec3& new_up)
{
	_up = glm::normalize(new_up);
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
	return _target;
}
glm::vec3 TargetCamera::up()
{
	return _up;
}
float TargetCamera::fov()
{
	return _fov;
}

glm::vec3 TargetCamera::rotateAroundTarget(double xy_angle, double xz_angle)
{
	xy_offset = glm::mod(xy_offset + xy_angle, glm::radians(360.0));
	xz_offset = glm::mod(xz_offset + xz_angle, glm::radians(360.0));

	_rotation = glm::angleAxis(float(xz_offset), _up);
	glm::vec3 direction = _target - _eye;
	if (std::abs(xy_offset) < 89.0)
	{
		_right = glm::normalize(glm::cross(direction, _up));
	}
	_rotation = glm::angleAxis(float(xy_offset), _right) * _rotation;
	return _eye;
}

void TargetCamera::recalcOffsets()
{
	glm::vec3 temp = _eye - _target;
	float len = glm::length(temp);
	if (len < std::numeric_limits<float>::epsilon())
	{
		xy_offset = xz_offset = 0;
		return;
	}
	else
	{
		xy_offset = glm::asin(temp.z / len);
	}
	if (std::abs(temp.x) < std::numeric_limits<float>::epsilon())
	{
		xz_offset = glm::radians(90.0f);
	}
	else
	{
		xz_offset = glm::atan(temp.y / temp.x);
	}
}
void TargetCamera::recalcUpVec()
{
	glm::vec3 direction = _target - _eye;
	glm::vec3 right = glm::cross(direction, _up);
	_up = glm::normalize(glm::cross(right, direction));
}