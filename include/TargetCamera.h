#pragma once
#include "zarevo_base.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

_ZRV_BEGIN

class ICameraStrategy
{
	
};
class TargetCamera
{
public:
	TargetCamera();
	TargetCamera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, const std::string& label = "");
	glm::mat4 getViewMatrix();

	void setEyeV(const glm::vec3& eye);
	void setTargerV(const glm::vec3& target);
	void setUpV(const glm::vec3& up);
	void setFov(float new_fov);
	glm::mat4 rotateAroundUpV(double yaw, double pitch);
	void rollUpV(double roll);
	void parallelOffset(double right, double up);
	void dirOffset(double offset);

	glm::vec3 eye();
	glm::vec3 target();
	glm::vec3 up();
	float fov();
	float length();

private:
	void recalcOffsets();
	glm::vec3 _eye, _center;
	glm::vec3 _up, _direction, _right, _rolled_up, _dpos;
	glm::quat _rotation, _roll_rot;
	float _fov;
	double _pitch, _yaw, _roll, _dir_offset;
	std::string _label;
};

_ZRV_END