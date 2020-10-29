#include "mWindow.h"
#include <Windows.h>
#include <fstream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>


void mWindow::initGl()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	std::vector<GLfloat> vertices =
	{
		-0.5f, 0.0f,  0.5f,
		-0.5f, 0.0f, -0.5f,
		 0.5f, 0.0f, -0.5f,
		 0.5f, 0.0f,  0.5f,
	};


	std::vector<GLfloat> texCoord =
	{
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};



	std::vector<GLuint> indices = {
		0, 1, 3,
		1, 2, 3
	};



//	mesh.initMesh(std::move(vertices), std::move(indices), std::move(texCoord), "zx.png");
	mesh.loadFromFile("2c.obj");
	mesh._model_matrix = glm::scale(mesh._model_matrix, glm::vec3{ 0.2f });
	mesh._model_matrix[3][3] = 1.0f;
	mesh._bbox.createBbox(mesh);
    _cam.fitInView(mesh._bbox);

	if (!m_prog.init("base_sh.vert", "base_sh.frag"))
	{
		ZRV_LOG << m_prog.getLog();
	}

	PROJECTION_CALL(Projection::Perspective)
	m_prog.use();

	GLint model_pos = m_prog.getUnifLoc("zrv_UModel");
	GLint view_pos = m_prog.getUnifLoc("zrv_UView");
	GLint proj_pos = m_prog.getUnifLoc("zrv_UProj");

	glUniformMatrix4fv(model_pos, 1, GL_FALSE, glm::value_ptr(mesh._model_matrix));
	glUniformMatrix4fv(view_pos, 1, GL_FALSE, glm::value_ptr(_cam.getViewMatrix()));

	m_prog.release();

	for (auto& str : zrv::glErrorHandler())
	{
		ZRV_LOG << str << std::endl;
	}
}


void mWindow::paintGl()
{
#ifdef _DEBUG
	Sleep(sleep * 1000.0);
#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (_draw_backface_as_wireframe_flag)
	{
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glCullFace(GL_BACK);
		mesh.useColor(false);
		mesh.drawMesh(m_prog);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glCullFace(GL_FRONT);
		mesh.useColor(true);
		mesh.drawMesh(m_prog);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glCullFace(GL_BACK);
		mesh.useColor(false);
		mesh.drawMesh(m_prog);

		glCullFace(GL_FRONT);
		mesh.useColor(true);
		mesh.drawMesh(m_prog);
	}
}

void mWindow::keyEvent(int key, int scancode, int action, int mode)
{
	_mod_shift = (mode == GLFW_MOD_SHIFT);

	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		_proj = Projection::Ortho;
		PROJECTION_CALL(Projection::Ortho)
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		_proj = Projection::Perspective;
		PROJECTION_CALL(Projection::Perspective)
	}
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		_draw_backface_as_wireframe_flag = !_draw_backface_as_wireframe_flag;
	}
	if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
	{
		_roll_flag = true;
	}
	if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE)
	{
		_roll_flag = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		_cam.fitInView(mesh._bbox);
		m_prog.use();
		GLint pos = m_prog.getUnifLoc("zrv_UView");
		glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(_cam.getViewMatrix()));
		m_prog.release();
	}

#ifdef _DEBUG
	if (key == GLFW_KEY_F5 && glfwGetTime() - key_pressed_time[GLFW_KEY_F5] > 0.5 && action == GLFW_PRESS && sleep > 0.1)
	{
		sleep -= 0.1;
		key_pressed_time[GLFW_KEY_F5] = glfwGetTime();
	}
	if (key == GLFW_KEY_F6 && glfwGetTime() - key_pressed_time[GLFW_KEY_F6] > 0.5 && action == GLFW_PRESS)
	{
		sleep += 0.1;
		key_pressed_time[GLFW_KEY_F6] = glfwGetTime();
	}
#endif
}

void mWindow::resizeEvent(int width, int height)
{
	glfwGetFramebufferSize(p_window, &_fw, &_fh);
	glViewport(0, 0, _fw, _fh);
	PROJECTION_CALL(_proj);
}
void mWindow::mouseMoveEvent(double xpos, double ypos)
{
	double dx = xpos - mouse.pos.x;
	double dy = ypos - mouse.pos.y;
	mouse.pos.x = xpos;
	mouse.pos.y = ypos;

	if (glfwGetMouseButton(p_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		//std::cout << __func__;
		if (_roll_flag)
		{
			if (xpos < _fw / 2.0)
			{
				_cam.rollUpV(-dy * mouse.sensivity / _fh);
			}
			else
			{
				_cam.rollUpV(+dy * mouse.sensivity / _fh);
			}
		}
		else
		{
			_cam.rotateAroundUpV(dx * mouse.sensivity / _fw, dy * mouse.sensivity / _fh);
		}
		m_prog.use();
		GLint pos = m_prog.getUnifLoc("zrv_UView");
		glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(_cam.getViewMatrix()));
		m_prog.release();
	}

	if (glfwGetMouseButton(p_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		double pan_coef = 2.0 * double(_cam.length()) * glm::tan(_cam.fov() / 2.0) / std::min<float>(_fw, _fh);
		_cam.parallelOffset(-dx * pan_coef, dy * pan_coef);
		m_prog.use();
		GLint pos = m_prog.getUnifLoc("zrv_UView");
		glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(_cam.getViewMatrix()));
		m_prog.release();
	}
}

void mWindow::mouseButtonEvent(int button, int action, int mods)
{
}
void mWindow::scrollEvent(double xoffset, double yoffset)
{
	if (glfwGetKey(p_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		float fov = _cam.fov() + glm::radians(-yoffset * 5.0f);
		if (fov < glm::radians(0.1f))
			fov = glm::radians(0.1f);
		if (fov > glm::radians(179.9f))
			fov = glm::radians(179.9f);
		_cam.setFov(fov);
		PROJECTION_CALL(_proj);
	}
	else
	{
		if (yoffset > 0.0)
			_cam.dirOffset(1 / 1.1);
		if (yoffset < 0.0)
			_cam.dirOffset(1.1);
		PROJECTION_CALL(_proj);
		m_prog.use();
		GLint pos = m_prog.getUnifLoc("zrv_UView");
		glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(_cam.getViewMatrix()));
		m_prog.release();
	}
}
