#pragma once
#include "zarevo_base.h"
#include "BBox.h"

_ZRV_BEGIN

/*! \brief Describes Target Camera object
*   \details Target Camera is camera, that can be described by three components: eye point, target (center) point and up vector
*/

class TargetCamera
{
public:
	TargetCamera();
	TargetCamera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, const std::string& label = "");
/*! \brief Returns transition matrix for camera
*   \return Homogenius 4x4 view matrix  
*/ 
	glm::mat4 getViewMatrix();
/*! \brief Returns rotation part of view matrix
*   \return 3x3 rotation matrix
*/
	glm::mat4 getRotationMatrix();

	void setEyeV(const glm::vec3& eye);
	void setTargerV(const glm::vec3& target);
	void setUpV(const glm::vec3& up);
	void setFov(float new_fov);
	void rotateAroundUpV(double yaw, double pitch);
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
	//!Gram–Schmidt process for _right, _up and _direction vectors
	void resetVectorsGS();
	glm::vec3 _eye, _center;
	glm::vec3 _up, _direction, _right, _rolled_up, _dpos;
	float _fov;
	double _dir_offset;
	std::string _label;
};

_ZRV_END