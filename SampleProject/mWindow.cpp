#include "mWindow.h"
#include <fstream>



void mWindow::initGl()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	std::vector<GLfloat> vertices =
	{
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};


	std::vector<GLfloat> texCoord =
	{
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
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
}


void mWindow::paintGl()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mesh.drawMesh(m_prog);
}

void mWindow::keyEvent(int key, int scancode, int action, int mode)
{
}

void mWindow::resizeEvent(int width, int height)
{
	glfwGetFramebufferSize(p_window, &_fw, &_fh);
	glViewport(0, 0, _fw, _fh);
}
void mWindow::mouseMoveEvent(double xpos, double ypos)
{
}

void mWindow::mouseButtonEvent(int button, int action, int mods)
{
}
void mWindow::scrollEvent(double xoffset, double yoffset)
{
}
