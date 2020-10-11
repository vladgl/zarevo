#pragma once
/* It is customary to use APIENTRY for OpenGL function pointer declarations on
 * all platforms.  Additionally, the Windows OpenGL header needs APIENTRY.
 */
#if !defined(APIENTRY)
#if defined(_WIN32)
#define APIENTRY __stdcall
#else
#define APIENTRY
#endif
#endif /* APIENTRY */

 // GL/gl.h needs this
#if !defined(WINGDIAPI) && defined(_WIN32)
#define WINGDIAPI __declspec(dllimport)
#endif // WINGDIAPI 


//if you want opengl wrapper only
#ifndef ZRV_LIB
#define ZRV_LIB
#define _ZRV_BEGIN namespace zrv {
#define _ZRV_END }
#define _ZRV_SOURCE using namespace zrv;
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <string>

_ZRV_BEGIN
typedef std::vector<std::string> ErrorList;

ErrorList glErrorHandler();
std::string getObjectTypeNameByIdentifier(GLenum identifier, const std::string& prefix);

enum AttribLoc
{
	A_POSITION = 0,
	A_NORMALS = 1,
	A_TEX_COORD = 2
};

_ZRV_END
