#include "mWindow.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
_INIT_PROJECTION_FUNC_BEGIN
PROJECTION_MAP_BEGIN(Projection::Ortho)

_aspect = _fw / float(_fh);
glm::mat4 projection{ 1.0f };
float scale = _cam.fov() / glm::radians(80.0f);
projection = glm::ortho(-_aspect * scale, _aspect * scale, -scale, scale, 0.5f, 5.0f);
m_prog.use();
GLint proj_pos = m_prog.getUnifLoc("projection");
glUniformMatrix4fv(proj_pos, 1, GL_FALSE, glm::value_ptr(projection));
m_prog.release();

PROJECTION_MAP_END

PROJECTION_MAP_BEGIN(Projection::Perspective)

_aspect = _fw / float(_fh);
glm::mat4 projection{ 1.0f };
projection = glm::perspective(_cam.fov(), _aspect, 0.01f, 100.0f);
m_prog.use();
GLint proj_pos = m_prog.getUnifLoc("projection");
glUniformMatrix4fv(proj_pos, 1, GL_FALSE, glm::value_ptr(projection));
m_prog.release();

PROJECTION_MAP_END

_INIT_PROJECTION_FUNC_END