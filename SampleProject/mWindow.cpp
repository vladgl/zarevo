#include "mWindow.h"
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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



	mesh.initMesh(vertices, indices, texCoord, "pic1");



	if (!m_prog.init("base_sh.vert", "base_sh.frag"))
	{
		ZRV_LOG << m_prog.getLog();
	}
	glEnable(GL_CULL_FACE);
	glm::mat4 model{ 1.0f }, view{ 1.0f }, projection{ 1.0f };

//	model = glm::translate(model, glm::vec3(.3f, 0.0f, 0.0f));

	PROJECTION_CALL(Projection::Perspective)
	m_prog.use();

	GLint model_pos = m_prog.getUnifLoc("model");
	GLint view_pos = m_prog.getUnifLoc("view");
	GLint proj_pos = m_prog.getUnifLoc("projection");
	ZRV_LOG << model_pos << view_pos << proj_pos;

	glUniformMatrix4fv(model_pos, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(view_pos, 1, GL_FALSE, glm::value_ptr(_cam.getViewMatrix()));

	m_prog.release();

	for (auto& str : zrv::glErrorHandler())
	{
		ZRV_LOG << str << std::endl;
	}
}


void mWindow::paintGl()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//mesh.useColor();
    //mesh.useTexture();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mesh.drawMesh(m_prog);
}

void mWindow::keyEvent(int key, int scancode, int action, int mode)
{
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
	double dy = - ypos + mouse.pos.y;
	mouse.pos.x = xpos;
	mouse.pos.y = ypos;
	if (glfwGetMouseButton(p_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		xy_offset -= dy * mouse.sensivity; 
		xz_offset = glm::mod(xz_offset + dx * mouse.sensivity, (double)glm::radians(360.0f));

		if (xy_offset > glm::radians(89.0))
			xy_offset = glm::radians(89.0);
		if (xy_offset < glm::radians(-89.0))
			xy_offset = glm::radians(-89.0);

		_cam.rotateEyeAroundZ(xy_offset, xz_offset);

		m_prog.use();
		GLint pos = m_prog.getUnifLoc("view");
		glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(_cam.getViewMatrix()));
		m_prog.release();
	}
}

void mWindow::mouseButtonEvent(int button, int action, int mods)
{
}
void mWindow::scrollEvent(double xoffset, double yoffset)
{
}
