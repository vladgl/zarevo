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
	glm::vec3 rotateAroundCenter(double xz_angle, double xy_angle, double roll);

	glm::vec3 eye();
	glm::vec3 target();
	glm::vec3 up();
	float fov();

private:
	void recalcOffsets();
	void recalcUpVec();
	glm::vec3 _eye, _center, _up, _right;
	glm::quat _rotation, _up_rot;
	float _length, _fov;
	double xy_offset, xz_offset, _roll;
	std::string _label;
};

_ZRV_END