#include "../include/TargetCamera.h"

_ZRV_SOURCE

TargetCamera::TargetCamera() :
	_eye(0.0f, 1.0f, 0.0f), _target(0.0f, 0.0f, 0.0f), _up(0.0f, 0.0f, 1.0f), _label("")
{
	_length = glm::length(_eye - _target);
}

TargetCamera::TargetCamera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, const std::string& label) :
	_eye(eye), _target(target), _up(up), _label(label)
{	
	_length = glm::length(_eye - _target);
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
}
void TargetCamera::setTargerV(const glm::vec3& new_target)
{
	_target = new_target;
	_length = glm::length(_eye - _target);
}
void TargetCamera::setUpV(const glm::vec3& new_up)
{
	_up = new_up;
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

glm::vec3 TargetCamera::rotateEyeAroundZ(float xy_angle, float xz_angle)
{
	_eye.x = _length * std::cosf(xy_angle) * std::cos(xz_angle);
	_eye.y = _length * std::cosf(xy_angle) * std::sin(xz_angle);
	_eye.z = _length * std::sinf(xy_angle);
	_eye += _target;
	return _eye;
}