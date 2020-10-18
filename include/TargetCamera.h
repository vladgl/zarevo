#pragma once
#include "zarevo_base.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
_ZRV_BEGIN

class TargetCamera
{
public:
	TargetCamera();
	TargetCamera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, const std::string& label = "");
	glm::mat4 getViewMatrix();

	void setEyeV(const glm::vec3& eye);
	void setTargerV(const glm::vec3& target);
	void setUpV(const glm::vec3& up);
	glm::vec3 rotateEyeAroundZ(float xy_angle, float xz_angle);

	glm::vec3 eye();
	glm::vec3 target();
	glm::vec3 up();

private:
	glm::vec3 _eye, _target, _up;
	float _length;
	std::string _label;
};

_ZRV_END