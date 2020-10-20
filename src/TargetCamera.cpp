#include "../include/TargetCamera.h"
#include <iostream>
_ZRV_SOURCE

TargetCamera::TargetCamera() :
	_eye(0.0f, 1.0f, 0.0f), _target(0.0f, 0.0f, 0.0f), _up(0.0f, 0.0f, 1.0f), _label(""), _fov(glm::radians(80.0f))
{
	_length = glm::length(_eye - _target);
	recalcOffsets();
}

TargetCamera::TargetCamera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, const std::string& label) :
	_eye(eye), _target(target), _up(up), _label(label), _fov(glm::radians(80.0f))
{	
	_length = glm::length(_eye - _target);
	recalcOffsets();
}

glm::mat4 TargetCamera::getViewMatrix()
{
	glm::vec3 vec{ _eye.x, _target.y - _eye.y, _eye.z };
	glm::mat4 view = glm::lookAt(vec, _target, _up);
	return view;
}

void TargetCamera::setEyeV(const glm::vec3& new_eye)
{
	_eye = new_eye;
	_length = glm::length(_eye - _target);
	recalcOffsets();
}
void TargetCamera::setTargerV(const glm::vec3& new_target)
{
	_target = new_target;
	_length = glm::length(_eye - _target);
	recalcOffsets();
}
void TargetCamera::setUpV(const glm::vec3& new_up)
{
	_up = new_up;
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
	xy_offset += xy_angle;
	xz_offset += xz_angle;

	if (xy_offset > glm::radians(89.0))
		xy_offset = glm::radians(89.0);
	if (xy_offset < glm::radians(-89.0))
		xy_offset = glm::radians(-89.0);

	_eye.x = _length * std::cosf(xy_offset) * std::cos(xz_offset);
	_eye.y = _length * std::cosf(xy_offset) * std::sin(xz_offset);
	_eye.z = _length * std::sinf(xy_offset);
	_eye += _target;
	return _eye;
}

void TargetCamera::recalcOffsets()
{
	glm::vec3 temp = _eye - _target;
	xy_offset = glm::asin(temp.z / glm::length(temp));
	xz_offset = glm::atan(temp.y / temp.x);	
}