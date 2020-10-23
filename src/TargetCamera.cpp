#include "../include/TargetCamera.h"
#include <iostream>
_ZRV_SOURCE

float get() { return 0.0f; }

TargetCamera::TargetCamera() :
	_eye(0.0f, 1.0f, 0.0f), _center(0.0f, 0.0f, 0.0f), _up(0.0f, 0.0f, 1.0f), _label(""), _fov(glm::radians(80.0f))
{
	_yaw = 0.0;
	_pitch = 0.0;
	_roll = 0.0;

	//Gram–Schmidt process
	_direction = glm::normalize(_center - _eye);
	_right = glm::normalize(glm::cross(_direction, _up));
	_up = glm::cross(_right, _direction);
	//

	//identity quaternions
	_rotation = glm::quat({0.0f, 0.0f, 0.0f});
	_roll_rot = glm::quat({ 0.0f, 0.0f, 0.0f });
	_rolled_up = _up;
	_dpos = glm::vec3(0.0f, 0.0f, 0.0f);
	_dir_offset = 1.0;
}

TargetCamera::TargetCamera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, const std::string& label) :
	_eye(eye), _center(target), _up(glm::normalize(up)), _label(label), _fov(glm::radians(80.0f))
{	
	_yaw = 0.0;
	_pitch = 0.0;
	_roll = 0.0;

	//Gram–Schmidt process
	_direction = glm::normalize(_center - _eye);
	_right = glm::normalize(glm::cross(_direction, _up));
	_up = glm::cross(_right, _direction);
	//

	//identity quaternions
	_rotation = glm::quat({ 0.0f, 0.0f, 0.0f });
	_roll_rot = glm::quat({ 0.0f, 0.0f, 0.0f });
	_rolled_up = _up;

	_dpos = glm::vec3(0.0f, 0.0f, 0.0f);
	_dir_offset = 1.0;
}


glm::mat4 TargetCamera::rotateAroundUpV(double yaw, double pitch)
{
	_pitch = glm::mod(_pitch + pitch, glm::radians(360.0));

	if (_pitch > glm::radians(89.9) && _pitch < glm::radians(271.1))
	{
		_yaw = glm::mod(_yaw - yaw, glm::radians(360.0));

	}
	else
	{
		_yaw = glm::mod(_yaw + yaw, glm::radians(360.0));

	}
	_rotation =  glm::angleAxis(float(_yaw), _up) * _roll_rot;

	double eps = 0.0002;
	if ((_pitch < glm::radians(90.0 - eps / 2.0) || _pitch > glm::radians(90 + eps / 2.0)) &&
		_pitch < glm::radians(270 - eps / 2.0) || _pitch > glm::radians(270 + eps/2.0))
	{
		_rotation = glm::angleAxis(float(_pitch), _right) * _rotation;
	}
	else
	{
		_rotation = glm::angleAxis(float(_pitch + glm::radians(eps)), _right) * _rotation;
	}

	return glm::mat4_cast(_rotation);
}

void TargetCamera::rollUpV(double roll)
{
	if (_pitch > glm::radians(89.9) && _pitch < glm::radians(271.1))
	{
		_roll = glm::mod(_roll - roll, glm::radians(360.0));
	}
	else
	{
		_roll = glm::mod(_roll + roll, glm::radians(360.0));
	}
	if (_yaw > glm::radians(89.9) && _yaw < glm::radians(271.1))
	{
		_roll_rot = glm::angleAxis(float(_roll), -_direction);
	}
	else
	{
		_roll_rot = glm::angleAxis(float(_roll), _direction);
	}

	_rolled_up = _up * _roll_rot;
}


void TargetCamera::parallelOffset(double right, double up)
{
//	_dpos += (-float(right) * _right * _rotation + float(up) * _rolled_up) ;
	_dpos += glm::vec3(float(right), 0.0f, float(up)) * _rotation;
}

void TargetCamera::dirOffset(double offset)
{
	_dir_offset *= offset;
	if (_dir_offset < 0.001)
		_dir_offset = 0.001;
}


glm::mat4 TargetCamera::getViewMatrix()
{
	glm::vec3 center = _center ;
	glm::vec3 eye = _center - float(_dir_offset) * (_center - _eye) * _rotation;
	glm::vec3 vec{ eye.x, center.y - eye.y, eye.z };
	glm::mat4 view{ 1.0f };
	if (_pitch > glm::radians(89.9999) && _pitch < glm::radians(269.9999))
		view = glm::lookAt(vec, center, -_rolled_up);
	else
		view = glm::lookAt(vec, center, _rolled_up);
	
	return view ;
}

void TargetCamera::setEyeV(const glm::vec3& new_eye)
{
	_eye = new_eye;
	_yaw = 0.0f;
	_pitch = 0.0f;
}
void TargetCamera::setTargerV(const glm::vec3& new_target)
{
	_center = new_target;
	_yaw = 0.0f;
	_pitch = 0.0f;
}
void TargetCamera::setUpV(const glm::vec3& new_up)
{
	_up = glm::normalize(new_up);
	_roll = 0.0;
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

void TargetCamera::recalcOffsets()
{
	glm::vec3 temp = _eye - _center;
	float len = glm::length(temp);
	if (len < std::numeric_limits<float>::epsilon())
	{
		_pitch = _yaw = 0;
		return;
	}
	else
	{
		_pitch = glm::asin(temp.z / len);
	}
	if (std::abs(temp.x) < std::numeric_limits<float>::epsilon())
	{
		_yaw = glm::radians(90.0f);
	}
	else
	{
		_yaw = glm::atan(temp.y / temp.x);
	}
}