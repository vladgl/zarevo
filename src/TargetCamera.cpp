#include "../include/TargetCamera.h"
#include <iostream>
_ZRV_SOURCE

std::ostream& operator<< (std::ostream& out, glm::vec3 vec)
{
	out << vec.x << " " << vec.y << " " << vec.z;
	return out;
}

void TargetCamera::resetVectorsGS()
{
	//Gram–Schmidt process
	_direction = glm::normalize(_center - _eye);
	_right = glm::normalize(glm::cross(_direction, _up));
	_up = _up - glm::dot(_up, _direction) / glm::dot(_direction, _direction) * _direction;
	_up = glm::normalize(_up);
	//
	_rolled_up = _up;
}

glm::mat4 TargetCamera::getRotationMatrix()
{
	return glm::mat4_cast(_rotation);
}
void TargetCamera::applyTransform()
{

}

TargetCamera::TargetCamera() :
	_eye(0.0f, -1.0f, 0.0f), _center(2.0f, 1.0f, 0.0f), _up(0.0f, 0.0f, 1.0f), _label(""), _fov(glm::radians(80.0f))
{
	_yaw = 0.0;
	_pitch = 0.0;
	_roll = 0.0;

	resetVectorsGS();

	//identity quaternions
	_rotation = glm::quat({0.0f, 0.0f, 0.0f});
	_roll_rot = glm::quat({ 0.0f, 0.0f, 0.0f });
	_dpos = glm::vec3(0.0f, 0.0f, 0.0f);
	_dir_offset = 1.0;
}

TargetCamera::TargetCamera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, const std::string& label) :
	_eye(eye), _center(target), _up(glm::normalize(up)), _label(label), _fov(glm::radians(80.0f))
{	
	_yaw = 0.0;
	_pitch = 0.0;
	_roll = 0.0;

	resetVectorsGS();

	//identity quaternions
	_rotation = glm::quat({ 0.0f, 0.0f, 0.0f });
	_roll_rot = glm::quat({ 0.0f, 0.0f, 0.0f });

	_dpos = glm::vec3(0.0f, 0.0f, 0.0f);
	_dir_offset = 1.0;
}


glm::mat4 TargetCamera::rotateAroundUpV(double yaw, double pitch)
{
	_pitch = glm::mod(_pitch + pitch, glm::radians(360.0));

	// if camera is upside down we inverse yaw
	if (_pitch > glm::radians(89.9) && _pitch < glm::radians(271.1))
	{
		_yaw = glm::mod(_yaw - yaw, glm::radians(360.0));
	}
	else
	{
		_yaw = glm::mod(_yaw + yaw, glm::radians(360.0));
	}

	glm::quat R_d = glm::angleAxis(float(_roll), _direction);
	glm::quat R_u = glm::angleAxis(float(_yaw), _up);
	glm::quat R_r = glm::angleAxis(float(_pitch), _right);

	_rotation = R_d * R_r * R_u;

	return glm::mat4_cast(_rotation);
}


glm::mat4 TargetCamera::getViewMatrix()
{
	glm::vec3 direction = float(_dir_offset) * (_center - _eye) * _rotation;
	glm::vec3 center = _center;
	glm::vec3 eye = center - direction;
	glm::mat4 view{ 1.0f };

	if (_pitch > glm::radians(89.99) && _pitch < glm::radians(270.01))
		view = glm::lookAt(eye, center, -_rolled_up);
	else
		view = glm::lookAt(eye, center, _rolled_up);

	return  glm::translate(view, _dpos);
}


void TargetCamera::rollUpV(double roll)
{
	glm::mat4 view = getViewMatrix();
	glm::vec3 direction = _direction;
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
		_roll_rot = glm::angleAxis(float(_roll), -direction );
	}
	else
	{
		_roll_rot = glm::angleAxis(float(_roll), direction ) ;
	}

	_rolled_up = _up * _roll_rot;
	rotateAroundUpV(0, 0);
}


void TargetCamera::parallelOffset(double right, double up)
{
	glm::mat4 view = getViewMatrix();
	//view^-1 == view^T, so columns is rows; note that glm::mat4 is column-major
	glm::vec3 vRight = { view[0][0], view[1][0], view[2][0] };
	glm::vec3 vUp = { view[0][1], view[1][1], view[2][1] };

	_dpos += -float(right) * vRight - float(up) * vUp;
}


void TargetCamera::dirOffset(double offset)
{
	_dir_offset *= offset;
	if (_dir_offset < 0.001)
		_dir_offset = 0.001;
}



void TargetCamera::fitInView(const AxisAlignedBB& bbox)
{
	_dpos = _center - bbox._center;

	glm::vec3 v = (bbox._bleft - bbox._tright) * _rotation;
	float lin = glm::length(glm::vec2(v.x, v.y));

	float d = lin / 2.0 / glm::tan(_fov / 2.0);
	_dir_offset = d / glm::length(_eye - _center) * 1.5;
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
