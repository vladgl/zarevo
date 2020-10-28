#pragma once
#define WIN32_LEAN_AND_MEAN

#ifndef ZRV_LIB

#define ZRV_LIB
#define _ZRV_BEGIN namespace zrv {
#define _ZRV_END }
#define _ZRV_SOURCE using namespace zrv;

#endif

#include "OpenGLWrapper/OglWrapper.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>