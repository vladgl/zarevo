#pragma once
#include "zarevo_base.h"
#include "BBox.h"

_ZRV_BEGIN

class TargetCamera
{
public:
	TargetCamera();
	TargetCamera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, const std::string& label = "");
	glm::mat4 getViewMatrix();
	glm::mat4 getRotationMatrix();
	void applyTransform();

	void setEyeV(const glm::vec3& eye);
	void setTargerV(const glm::vec3& target);
	void setUpV(const glm::vec3& up);
	void setFov(float new_fov);
	glm::mat4 rotateAroundUpV(double yaw, double pitch);
	void rollUpV(double roll);
	void parallelOffset(double right, double up);
	void dirOffset(double offset);
	void fitInView(const AxisAlignedBB& bbox);

	glm::vec3 eye();
	glm::vec3 target();
	glm::vec3 up();
	float fov();
	float length();

private:
	void resetVectorsGS();
	glm::vec3 _eye, _center;
	glm::vec3 _up, _direction, _right, _rolled_up, _dpos;
	glm::quat _rotation, _roll_rot, _pitch_yaw_rot;
	float _fov;
	double _pitch, _yaw, _roll, _dir_offset;
	std::string _label;
};

_ZRV_END